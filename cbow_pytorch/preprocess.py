import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
import string

from torch.utils.data import Dataset
from torch.utils.data import DataLoader
from collections import Counter

########

def custom_standardization(text: str) -> str:
    text = text.lower()
    translator = str.maketrans('', '', string.punctuation)
    return text.translate(translator)


def read_data(data_path):
    with open(data_path, "r", encoding="utf-8") as f:
        input_data = f.read()
    data = custom_standardization(input_data)
    return data.split()


def get_index(data_path):
    # Count word frequencies
    tokens = read_data(data_path)
    word_counts = Counter(tokens)

    vocab = {word: idx for idx, (word, _) in enumerate(word_counts.items(), start=1)}
    word2idx = {word: idx for idx, (word, _) in enumerate(vocab.items())}
    idx2word = {idx: word for word, idx in word2idx.items()}

    return word2idx, idx2word


def get_sequence(data_path: str):
    tokens = read_data(data_path)
    print(" -- Total number of tokens:", len(tokens))
    
    word2idx, _ = get_index(data_path)
    vocab_size = len(word2idx)
    print(" -- Vocab size is:", vocab_size)

    sequence = torch.tensor(
        [word2idx[word] for word in tokens],
        dtype=torch.long
    )

    return sequence, vocab_size


def resize_embedding(old_emb, new_vocab_size):
    old_weight = old_emb.weight.data
    old_vocab_size, emb_dim = old_weight.shape

    print(f" -- Old embeddings size: {old_weight.shape}")
    
    new_emb = nn.Embedding(new_vocab_size, emb_dim)
    new_emb.weight.data[:old_vocab_size] = old_weight
    nn.init.normal_(new_emb.weight.data[old_vocab_size:], mean=0, std=0.02)

    print(f" -- New embeddings size:{new_emb.weight.data.shape}")
    
    return new_emb



def resize_linear(old_fc, new_vocab_size):
    old_weight = old_fc.weight.data
    old_bias = old_fc.bias.data

    old_vocab_size, emb_dim = old_weight.shape

    new_fc = nn.Linear(emb_dim, new_vocab_size)
    new_fc.weight.data[:old_vocab_size] = old_weight
    new_fc.bias.data[:old_vocab_size] = old_bias

    nn.init.zeros_(new_fc.weight.data[old_vocab_size:])
    nn.init.zeros_(new_fc.bias.data[old_vocab_size:])

    return new_fc