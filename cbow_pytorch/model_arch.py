import torch
import torch.nn as nn
import torch.nn.functional as F

from torch.utils.data import Dataset


class CBOWDataset(Dataset):
    def __init__(self, sequence: torch.Tensor, window_size: int):
        self.sequence    = sequence
        self.window_size = window_size
        
        # Number of valid centre-word positions
        self.n = len(sequence) - 2 * window_size

    def __len__(self) -> int:
        return self.n

    def __getitem__(self, idx: int):
        i = idx + self.window_size
        context = torch.cat((
            self.sequence[i - self.window_size : i],
            self.sequence[i + 1 : i + self.window_size + 1]
        ))
        return context, self.sequence[i]


# ─────────────────────────────────────────────────────────────────────────────


class CBOWModel(nn.Module):
    def __init__(self, vocab_size: int, embedding_dim: int):
        super().__init__()
        self.embedding = nn.Embedding(
            num_embeddings=vocab_size,
            embedding_dim=embedding_dim
        )
        self.fc = nn.Linear(embedding_dim, vocab_size)

    def forward(self, x: torch.Tensor) -> torch.Tensor:
        embeds     = self.embedding(x)
        mean_embed = embeds.mean(dim=1)
        return self.fc(mean_embed)


# ─────────────────────────────────────────────────────────────────────────────


class CBOWModelNS(nn.Module):
    def __init__(self, vocab_size: int, embedding_dim: int):
        super().__init__()

        self.in_embed  = nn.Embedding(vocab_size, embedding_dim)
        self.out_embed = nn.Embedding(vocab_size, embedding_dim)

        nn.init.uniform_(self.in_embed.weight, -0.5 / embedding_dim, 0.5 / embedding_dim)
        nn.init.uniform_(self.out_embed.weight, -0.5 / embedding_dim, 0.5 / embedding_dim)

    # Alias so legacy code accessing model.embedding still works.
    @property
    def embedding(self) -> nn.Embedding:
        return self.in_embed

    def forward(self, context: torch.Tensor, target: torch.Tensor, neg_samples: torch.Tensor) -> torch.Tensor:
        h = self.in_embed(context).mean(dim=1)

        # Positive
        v_pos     = self.out_embed(target)
        pos_score = F.logsigmoid((h * v_pos).sum(dim=1))

        # Negative
        v_neg     = self.out_embed(neg_samples)
        neg_score = F.logsigmoid(-(h.unsqueeze(1) * v_neg).sum(dim=2)).sum(dim=1)

        return -(pos_score + neg_score).mean()

    def get_input_embeddings(self) -> torch.Tensor:
        return self.in_embed.weight.data

    def get_output_embeddings(self) -> torch.Tensor:
        return self.out_embed.weight.data
