import torch
import torch.nn as nn
import string
import os

from collections import Counter
from google.cloud import storage

# -------------------

def custom_standardization(text: str) -> str:
    text = text.lower()
    translator = str.maketrans('', '', string.punctuation)
    return text.translate(translator)

# -------------------

def read_data(data_path):
    with open(data_path, "r", encoding="utf-8") as f:
        input_data = f.read()
    data = custom_standardization(input_data)
    return data.split()

# -------------------

def get_index(data_path: str, min_freq: int = 5):
    tokens      = read_data(data_path)
    word_counts = Counter(tokens)

    # Filter rare words; sort for stable, consistent index assignment
    vocab = sorted(w for w, c in word_counts.items() if c >= min_freq)

    word2idx = {"<UNK>": 0}
    word2idx.update({w: i + 1 for i, w in enumerate(vocab)})
    idx2word = {i: w for w, i in word2idx.items()}

    return word2idx, idx2word, word_counts

# -------------------

def get_sequence(data_path: str, min_freq: int = 5):
    tokens = read_data(data_path)
    word2idx, _, word_counts = get_index(data_path, min_freq=min_freq)

    unk_idx    = word2idx["<UNK>"]
    vocab_size = len(word2idx)

    sequence = torch.tensor(
        [word2idx.get(word, unk_idx) for word in tokens],
        dtype=torch.long
    )

    return sequence, vocab_size, word_counts

# -------------------

def build_noise_dist(word2idx: dict, word_counts: Counter, power: float = 0.75) -> torch.Tensor:
    vocab_size = len(word2idx)
    freq       = torch.zeros(vocab_size)

    for word, idx in word2idx.items():
        freq[idx] = word_counts.get(word, 0) ** power

    total = freq.sum()
    if total > 0:
        freq /= total

    return freq

# -------------------

def resize_embedding(old_emb, new_vocab_size):
    old_weight = old_emb.weight.data
    old_vocab_size, emb_dim = old_weight.shape

    print(f" -- Old embeddings size: {old_weight.shape}")

    new_emb = nn.Embedding(new_vocab_size, emb_dim)
    new_emb.weight.data[:old_vocab_size] = old_weight
    nn.init.normal_(new_emb.weight.data[old_vocab_size:], mean=0, std=0.02)

    print(f" -- New embeddings size:{new_emb.weight.data.shape}")

    return new_emb

# -------------------

def resize_linear(old_fc, new_vocab_size):
    old_weight = old_fc.weight.data
    old_bias = old_fc.bias.data

    old_vocab_size, emb_dim = old_weight.shape

    new_fc = nn.Linear(emb_dim, new_vocab_size)
    new_fc.weight.data[:old_vocab_size] = old_weight
    new_fc.bias.data[:old_vocab_size] = old_bias

    nn.init.normal_(new_fc.weight.data[old_vocab_size:], mean=0, std=0.02)
    nn.init.zeros_(new_fc.bias.data[old_vocab_size:])

    return new_fc

# -------------------

def download_model_from_gcs(
        bucket_name,
        download_dir
    ):
    client = storage.Client.from_service_account_json("key.json")
    bucket = client.bucket(bucket_name)

    blobs = bucket.list_blobs()
    versions = set()

    for blob in blobs:
        folder = blob.name.split('/')[0]
        if folder.startswith('version-'):
            versions.add(folder)

    # --- Find the latest version by number ---
    try:
        latest_version = max(versions, key=lambda x: int(x.split('-')[1]))
        print("Latest version folder:", latest_version)
    except ValueError:
        print(f"No model versions found in {bucket_name} GCS bucket.")
        return

    if not os.path.isdir(download_dir):
        os.makedirs(download_dir, exist_ok=True)
        print("Model folder has been created")

    version_num = latest_version.split('-')[1]

    for blob in bucket.list_blobs(prefix=f"{latest_version}/"):
        if blob.name.endswith('/'):
            continue

        filename = os.path.basename(blob.name).replace(f"_v{version_num}", "")
        local_path = os.path.join(download_dir, filename)

        if not os.path.exists(local_path):
            print(f"Downloading {blob.name} -> {local_path}")
            blob.download_to_filename(local_path)
        else:
            print(f"Model already exists at {local_path}")

# -------------------

def download_data_from_gcs(
        bucket_name,
        data_path
    ):
    client = storage.Client.from_service_account_json("key.json")
    bucket = client.bucket(bucket_name)

    if not os.path.exists(f"data/{data_path}"):
        try:
            blob = bucket.blob(data_path)
            blob.download_to_filename(f"data/{data_path}")
            print(f"\n -- File has been downloaded in 'data/{data_path}'")
        except:
            print(f" ====== Error: failed to download '{data_path}' file from {bucket_name} GCS bucket.")
    else:
        print(f"Data already exists at data/{data_path}")

# -------------------

def save_model_to_gcs(
        bucket_name,
        model_dir: str = "./model",
    ):
    client = storage.Client.from_service_account_json("key.json")
    bucket = client.bucket(bucket_name)

    blobs = client.list_blobs(bucket_name, delimiter='/')
    _ = list(blobs)

    try:
        latest_version = max([int(v.split('-')[1].replace('/', '')) for v in blobs.prefixes])
    except ValueError as e:
        print("No model versions found. Starting with version 1")
        latest_version = 0

    destination_prefix = f"version-{latest_version + 1}"
    print(f" -- New model version {latest_version + 1} will be saved in {destination_prefix}/ folder")

    for root, dirs, files in os.walk(model_dir):
        for file in files:
            local_path = os.path.join(root, file)

            relative_path = os.path.relpath(local_path, model_dir)
            gcs_path = os.path.join(destination_prefix, relative_path).replace("\\", "/")

            blob = bucket.blob(gcs_path)
            blob.upload_from_filename(local_path, timeout=300)

            print(f"Uploaded {local_path} → gs://{bucket_name}/{gcs_path}")
