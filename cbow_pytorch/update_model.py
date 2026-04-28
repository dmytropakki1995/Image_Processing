import json
import copy
import hashlib
import os
import re
import time
import torch
import torch.nn as nn
import torch.optim as optim

from collections import Counter
from torch.utils.data import DataLoader

import preprocess
import model_arch


# ── Helpers ───────────────────────────────────────────────────────────────────

def _file_hash(path: str) -> str:
    """MD5 hash of a file — used to detect duplicate ingestion."""
    with open(path, "rb") as f:
        return hashlib.md5(f.read()).hexdigest()


def _tokenize(path: str) -> list[str]:
    with open(path, "r", encoding="utf-8") as f:
        text = f.read()
    # remove special characters
    text = re.sub('[^A-Za-z0-9]+', ' ', text)
    # remove 1 letter words
    text = re.sub(r'(?:^| )\w(?:$| )', ' ', text).strip()
    # lower all characters
    text = text.lower()

    return text.split()


# ── Core flow ─────────────────────────────────────────────────────────────────

def load_config(config_path: str) -> dict:
    with open(config_path, "r") as f:
        cfg = json.load(f)

    if "embedding_size" in cfg and "embedding_shape" not in cfg:
        cfg["embedding_shape"] = [None, cfg["embedding_size"]]

    cfg.setdefault("sequence", [])
    cfg.setdefault("sequence_path", None)
    cfg.setdefault("ingested_sources", [])
    cfg.setdefault("word2idx", {})
    cfg.setdefault("word_counts", {})
    cfg.setdefault("model_type", "CBOWModel")

    return cfg


def load_model(cfg: dict, model_dir: str) -> nn.Module:
    vocab_size = len(cfg["word2idx"])
    emb_dim    = cfg["embedding_shape"][1]

    model_type = cfg.get("model_type", "CBOWModel")
    if model_type == "CBOWModelNS":
        model = model_arch.CBOWModelNS(vocab_size, emb_dim)
    else:
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


def ingest_new_text(cfg: dict, new_data_path: str, sequence_path: str, min_freq: int = 0) -> tuple[dict, list[str]]:
    
    # Check the data hash to avoid duplicate ingestion
    src_hash = _file_hash(new_data_path)
    if src_hash in cfg["ingested_sources"]:
        print(f" -- Skipping '{new_data_path}': already ingested (hash match)")
        return cfg, []

    new_tokens = _tokenize(new_data_path)
    if not new_tokens:
        print(" -- New file is empty; nothing to do")
        return cfg, []

    merged_counts = Counter(cfg.get("word_counts", {}))
    merged_counts.update(Counter(new_tokens))

    filtered_words = {w for w, c in merged_counts.items() if c >= min_freq}
    merged_vocab   = copy.deepcopy(cfg["word2idx"])

    if "<UNK>" not in merged_vocab:
        merged_vocab["<UNK>"] = len(merged_vocab)

    next_idx      = len(merged_vocab)
    new_word_count = 0
    for word in sorted(filtered_words):
        if word not in merged_vocab:
            merged_vocab[word] = next_idx
            next_idx      += 1
            new_word_count += 1

    print(f" -- Existing vocab size : {len(cfg['word2idx']):,}")
    print(f" -- New words added     : {new_word_count:,}")
    print(f" -- Merged vocab size   : {len(merged_vocab):,}")

    unk_idx = merged_vocab["<UNK>"]

    if os.path.exists(sequence_path):
        existing_seq = torch.load(sequence_path, map_location="cpu").tolist()
    else:
        existing_seq = cfg.get("sequence", [])

    new_indices  = [merged_vocab.get(t, unk_idx) for t in new_tokens]
    full_sequence = existing_seq + new_indices

    torch.save(torch.tensor(full_sequence, dtype=torch.long), sequence_path)
    print(f" -- Sequence length: {len(full_sequence):,} tokens  (saved → {sequence_path})")

    cfg["word2idx"]      = merged_vocab
    cfg["word_counts"]   = dict(merged_counts)
    cfg["sequence_path"] = sequence_path
    cfg["ingested_sources"].append(src_hash)

    cfg.pop("sequence", None)

    return cfg, new_tokens


def resize_model_if_needed(model: nn.Module, cfg: dict) -> nn.Module:
    new_vocab_size = len(cfg["word2idx"])
    old_vocab_size = model.embedding.num_embeddings

    if new_vocab_size > old_vocab_size:
        print(f" -- Resizing model: {old_vocab_size:,} → {new_vocab_size:,} vocab entries")

        if isinstance(model, model_arch.CBOWModelNS):
            model.in_embed  = preprocess.resize_embedding(model.in_embed,  new_vocab_size)
            model.out_embed = preprocess.resize_embedding(model.out_embed, new_vocab_size)
        else:
            model.embedding = preprocess.resize_embedding(model.embedding, new_vocab_size)
            model.fc        = preprocess.resize_linear(model.fc, new_vocab_size)

        cfg["embedding_shape"] = [new_vocab_size, model.embedding.weight.shape[1]]
    else:
        print(" -- Vocab unchanged; no layer resizing needed")

    return model


def train(
    model:   nn.Module,
    cfg:     dict,
    device:  torch.device,
    epochs:  int   = 5,
    lr:      float = 0.001,
    num_neg: int   = 5,
) -> nn.Module:

    seq_path = cfg.get("sequence_path")
    if seq_path and os.path.exists(seq_path):
        sequence = torch.load(seq_path, map_location="cpu")
    else:
        sequence = torch.tensor(cfg.get("sequence", []), dtype=torch.long)

    window_size = cfg["window_size"]
    batch_size  = cfg["batch_size"]

    dataset = model_arch.CBOWDataset(sequence, window_size)

    use_pin_memory = torch.cuda.is_available()
    num_workers    = 4
    dataloader = DataLoader(
        dataset,
        batch_size=batch_size,
        shuffle=True,
        num_workers=num_workers,
        pin_memory=use_pin_memory,
        persistent_workers=(num_workers > 0),
    )

    model     = model.to(device)
    optimizer = optim.Adam(model.parameters(), lr=lr)
    is_neg    = isinstance(model, model_arch.CBOWModelNS)

    if is_neg:
        word_counts = Counter(cfg.get("word_counts", {}))
        noise_dist  = preprocess.build_noise_dist(cfg["word2idx"], word_counts)
        criterion   = None
    else:
        noise_dist  = None
        criterion   = nn.CrossEntropyLoss()

    print(f"\n -- Training for {epochs} epoch(s) on {len(dataset):,} samples "
          f"({'NEG k=' + str(num_neg) if is_neg else 'CrossEntropy'}, "
          f"device={str(device).upper()})\n")

    for epoch in range(epochs):
        t0         = time.perf_counter()
        total_loss = 0.0

        for context, target in dataloader:
            context = context.to(device)
            target  = target.to(device)
            optimizer.zero_grad()

            if is_neg:
                neg_idx     = torch.multinomial(
                    noise_dist,
                    num_samples=num_neg * context.shape[0],
                    replacement=True,
                )
                neg_samples = neg_idx.view(context.shape[0], num_neg).to(device)
                loss = model(context, target, neg_samples)
            else:
                logits = model(context)
                loss   = criterion(logits, target)

            loss.backward()
            optimizer.step()
            total_loss += loss.item()

        elapsed = time.perf_counter() - t0
        print(f"   Epoch {epoch + 1:>3}/{epochs}  "
              f"loss={total_loss:.4f}  time={elapsed:.1f}s")

    return model.to("cpu")


def save(
    model: nn.Module,
    cfg: dict,
    model_dir: str = "./model",
    config_path: str = "./model/model_ns_config.json"
):
    model_filename = cfg.get("model_path", "model_cpu.pt").split("/")[-1]
    weights_path   = f"{model_dir}/{model_filename}"

    torch.save(model.state_dict(), weights_path)

    cfg["model_path"]    = weights_path
    cfg["model_type"]    = model.__class__.__name__
    cfg["embedding_shape"] = list(model.embedding.weight.data.shape)

    save_cfg = {k: v for k, v in cfg.items() if k != "sequence"}

    with open(config_path, "w") as f:
        json.dump(save_cfg, f, indent=4)

    print(f"\n -- Weights  saved → {weights_path}")
    print(f" -- Config   saved → {config_path}")


# ── Entry point ───────────────────────────────────────────────────────────────

def init_model(
    new_data_path: str,
    model_dir:     str = "./model",
    config_path:   str = "./model/model_ns_config.json",
):
    device = (
        torch.device("mps")  if torch.backends.mps.is_available() else
        torch.device("cuda") if torch.cuda.is_available()         else
        torch.device("cpu")
    )

    print("=" * 55)
    print(f" CBOW Incremental Update")
    print(f"   new data : {new_data_path}")
    print(f"   device   : {str(device).upper()}")
    print("=" * 55)

    sequence_path = os.path.join(model_dir, "sequence.pt")

    cfg = load_config(config_path)
    model = load_model(cfg, model_dir)

    cfg, _ = ingest_new_text(cfg, new_data_path, sequence_path)
    model = resize_model_if_needed(model, cfg)

    print("\n -- Done ✓")
    return model, cfg


def init_first_model(
    data_path:     str,
    model_dir:     str = "./model",
    embedding_dim: int = 256,
    window_size:   int = 2,
    batch_size:    int = 1024,
    min_freq:      int = 5,
):
    os.makedirs(model_dir, exist_ok=True)
    config_path   = os.path.join(model_dir, "model_ns_config.json")
    sequence_path = os.path.join(model_dir, "sequence.pt")

    print("=" * 55)
    print(f" Initialising New CBOW-NS Model")
    print(f"   data path : data/{data_path}")
    print(f"   model dir : {model_dir}")
    print("=" * 55)

    cfg = {
        "embedding_shape":  [None, embedding_dim],
        "window_size":      window_size,
        "batch_size":       batch_size,
        "word2idx":         {},
        "word_counts":      {},
        "sequence_path":    sequence_path,
        "ingested_sources": [],
        "model_type":       "CBOWModelNS",
        "model_path":       "model_cpu.pt"
    }

    cfg, _ = ingest_new_text(cfg, f"data/{data_path}", sequence_path, min_freq=min_freq)

    vocab_size = len(cfg["word2idx"])
    print(f" -- Creating fresh CBOWModelNS (vocab={vocab_size:,}, dim={embedding_dim})")
    model = model_arch.CBOWModelNS(vocab_size, embedding_dim)

    save(model, cfg, model_dir, config_path)

    print("\n -- First-time initialisation done ✓")
    return model, cfg
