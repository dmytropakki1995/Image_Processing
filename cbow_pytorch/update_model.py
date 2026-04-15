"""
update_model.py
---------------
Incremental CBOW update flow.

Steps:
  1. Load model config (word2idx, hyperparams, ingested sources)
  2. Load pretrained model weights
  3. Read new text file → tokenize
  4. Extend vocab with new words (if any)
  5. Extend sequence with new text (re-indexed via merged vocab)
  6. Resize model layers if vocab grew
  7. Train / fine-tune on the full sequence
  8. Save updated model weights + model_config.json
"""

import json
import copy
import hashlib
import string
import time
import torch
import torch.nn as nn
import torch.optim as optim

from torch.utils.data import DataLoader

import preprocess
import model_arch


# ── Helpers ──────────────────────────────────────────────────────────────────

def _file_hash(path: str) -> str:
    """MD5 hash of a file — used to detect duplicate ingestion."""
    with open(path, "rb") as f:
        return hashlib.md5(f.read()).hexdigest()


def _tokenize(path: str) -> list[str]:
    """Lowercase + strip punctuation + split."""
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    text = text.lower()
    translator = str.maketrans("", "", string.punctuation)
    return text.translate(translator).split()


# ── Core flow ─────────────────────────────────────────────────────────────────

def load_config(config_path: str) -> dict:
    with open(config_path, "r") as f:
        cfg = json.load(f)

    # Normalise legacy key names
    if "embedding_size" in cfg and "embedding_shape" not in cfg:
        cfg["embedding_shape"] = [None, cfg["embedding_size"]]

    cfg.setdefault("sequence", [])
    cfg.setdefault("ingested_sources", [])
    cfg.setdefault("word2idx", {})

    return cfg


def load_model(cfg: dict, model_dir: str) -> nn.Module:
    vocab_size  = len(cfg["word2idx"])
    emb_dim     = cfg["embedding_shape"][1]

    model = model_arch.CBOWModel(vocab_size, emb_dim)

    model_path = cfg.get("model_path")
    if model_path:
        full_path = f"{model_dir}/{model_path.split('/')[-1]}"
        try:
            model.load_state_dict(torch.load(full_path, map_location="cpu"))
            print(f" -- Loaded weights from {full_path}")
        except Exception as e:
            print(f" -- Could not load weights ({e}); starting fresh")
    else:
        print(" -- No model_path in config; starting fresh")

    return model


def ingest_new_text(cfg: dict, new_data_path: str) -> tuple[dict, list[str]]:
    """
    Returns:
        updated cfg  (vocab + sequence extended, ingested_sources updated)
        new_tokens   (raw token list from the new file)
    """
    src_hash = _file_hash(new_data_path)
    if src_hash in cfg["ingested_sources"]:
        print(f" -- Skipping '{new_data_path}': already ingested (hash match)")
        return cfg, []

    new_tokens = _tokenize(new_data_path)
    if not new_tokens:
        print(" -- New file is empty; nothing to do")
        return cfg, []

    # ── Extend vocab ──────────────────────────────────────────────────────
    merged_vocab = copy.deepcopy(cfg["word2idx"])
    next_idx     = len(merged_vocab)
    new_word_count = 0

    for token in new_tokens:
        if token not in merged_vocab:
            merged_vocab[token] = next_idx
            next_idx += 1
            new_word_count += 1

    print(f" -- Existing vocab size : {len(cfg['word2idx'])}")
    print(f" -- New words found     : {new_word_count}")
    print(f" -- Merged vocab size   : {len(merged_vocab)}")

    # ── Extend sequence (using merged vocab so indices are consistent) ────
    new_indices = [merged_vocab[t] for t in new_tokens]
    cfg["sequence"].extend(new_indices)
    print(f" -- Total sequence length after merge: {len(cfg['sequence'])}")

    cfg["word2idx"] = merged_vocab
    cfg["ingested_sources"].append(src_hash)

    return cfg, new_tokens


def resize_model_if_needed(model: nn.Module, cfg: dict) -> nn.Module:
    """Grow embedding + linear layers if vocab expanded."""
    new_vocab_size = len(cfg["word2idx"])
    old_vocab_size = model.embedding.num_embeddings

    if new_vocab_size > old_vocab_size:
        print(f" -- Resizing model: {old_vocab_size} → {new_vocab_size} vocab entries")
        model.embedding = preprocess.resize_embedding(model.embedding, new_vocab_size)
        model.fc        = preprocess.resize_linear(model.fc, new_vocab_size)
        cfg["embedding_shape"] = list(model.embedding.weight.data.shape)
    else:
        print(" -- Vocab unchanged; no layer resizing needed")

    return model


def train(
    model:      nn.Module,
    cfg:        dict,
    device:     torch.device,
    epochs:     int = 5,
    lr:         float = 0.001,
) -> nn.Module:
    sequence   = torch.tensor(cfg["sequence"], dtype=torch.long)
    window_size = cfg["window_size"]
    batch_size  = cfg["batch_size"]

    dataset    = model_arch.CBOWDataset(sequence, window_size)
    dataloader = DataLoader(dataset, batch_size=batch_size, shuffle=True)

    model      = model.to(device)
    criterion  = nn.CrossEntropyLoss()
    optimizer  = optim.Adam(model.parameters(), lr=lr)

    print(f"\n -- Training for {epochs} epoch(s) on {len(dataset)} samples "
          f"(device={str(device).upper()})\n")

    for epoch in range(epochs):
        t0 = time.perf_counter()
        total_loss = 0.0

        for context, target in dataloader:
            context, target = context.to(device), target.to(device)
            optimizer.zero_grad()
            logits = model(context)
            loss   = criterion(logits, target)
            loss.backward()
            optimizer.step()
            total_loss += loss.item()

        elapsed = time.perf_counter() - t0
        print(f"   Epoch {epoch + 1:>3}/{epochs}  "
              f"loss={total_loss:.4f}  time={elapsed:.1f}s")

    return model.to("cpu")


def save(model: nn.Module, cfg: dict, model_dir: str, config_path: str):
    """Save model weights and updated config JSON."""
    model_filename = cfg.get("model_path", "model_cpu.pt").split("/")[-1]
    weights_path   = f"{model_dir}/{model_filename}"

    torch.save(model.state_dict(), weights_path)
    cfg["model_path"] = weights_path

    # Keep embedding_shape in sync
    cfg["embedding_shape"] = list(model.embedding.weight.data.shape)

    with open(config_path, "w") as f:
        json.dump(cfg, f, indent=4)

    print(f"\n -- Weights  saved → {weights_path}")
    print(f" -- Config   saved → {config_path}")


# ── Entry point ───────────────────────────────────────────────────────────────

def initiate_model(
    new_data_path: str,
    model_dir:     str  = "./model",
    config_path:   str  = "./model/model_config.json",
    # epochs:        int  = 5,
    # lr:            float = 0.001,
):
    """
    Full incremental update pipeline.

    Args:
        new_data_path : path to the new text file
        model_dir     : directory containing weights + config
        config_path   : path to model_config.json
        epochs        : number of fine-tuning epochs
        lr            : learning rate
    """
    device = (
        torch.device("mps")  if torch.backends.mps.is_available() else
        torch.device("cuda") if torch.cuda.is_available()          else
        torch.device("cpu")
    )

    print("=" * 55)
    print(f" CBOW Incremental Update")
    print(f"   new data : {new_data_path}")
    print(f"   device   : {str(device).upper()}")
    print("=" * 55)

    # 1. Load config
    cfg   = load_config(config_path)

    # 2. Load model
    model = load_model(cfg, model_dir)

    # 3. Ingest new text (extend vocab + sequence, deduplicate via hash)
    cfg, _ = ingest_new_text(cfg, new_data_path)

    # 4. Resize layers if vocab grew
    model = resize_model_if_needed(model, cfg)

    print("\n -- Done ✓")
    return model, cfg

