import torch
import torch.nn as nn

from torch.utils.data import Dataset


class CBOWDataset(Dataset):
    def __init__(self, sequence, window_size):
        self.data = []
        self.window_size = window_size

        for i in range(window_size, len(sequence) - window_size):
            context = torch.cat((
                sequence[i - window_size:i],
                sequence[i + 1:i + window_size + 1]
            ))
            target = sequence[i]
            self.data.append((context, target))

    def __len__(self):
        return len(self.data)

    def __getitem__(self, idx):
        context, target = self.data[idx]
        return context, target


import torch
import torch.nn as nn
import torch.nn.functional as F

class CBOW_NS(nn.Module):
    def __init__(self, vocab_size, embedding_dim):
        super().__init__()
        self.in_embed = nn.Embedding(vocab_size, embedding_dim)   # context embeddings
        self.out_embed = nn.Embedding(vocab_size, embedding_dim)  # target embeddings

    def forward(self, context_idxs, target_idxs, negative_idxs):
        """
        context_idxs: (batch_size, context_size)
        target_idxs: (batch_size,)
        negative_idxs: (batch_size, K)
        """
        
        # 1) Build context representation
        context_vecs = self.in_embed(context_idxs)         # (B, C, D)
        h = context_vecs.mean(dim=1)                        # (B, D)

        # 2) Positive score
        target_vecs = self.out_embed(target_idxs)           # (B, D)
        pos_score = torch.sum(h * target_vecs, dim=1)       # (B,)
        pos_loss = F.logsigmoid(pos_score)                  # log σ(h·v_pos)

        # 3) Negative scores
        neg_vecs = self.out_embed(negative_idxs)            # (B, K, D)
        neg_score = torch.bmm(neg_vecs, h.unsqueeze(2)).squeeze(2)  # (B, K)
        neg_loss = F.logsigmoid(-neg_score).sum(dim=1)      # Σ log σ(-h·v_neg)

        # 4) Combine losses
        loss = -(pos_loss + neg_loss).mean()
        return loss


class CBOWModel(nn.Module):
    def __init__(self, vocab_size, embedding_dim):
        super(CBOWModel, self).__init__() 
        self.embeddings = nn.Embedding(vocab_size, embedding_dim) 
        self.linear1 = nn.Linear(embedding_dim, 128)
        self.activation = nn.GELU()
        self.linear2 = nn.Linear(128, vocab_size)

    def forward(self, context_idxs):
        # Shape: (context_size,)
        embeds = self.embeddings(context_idxs)  # (context_size, embedding_dim)

        # CBOW: average embeddings
        embeds = embeds.mean(dim=0).view(1, -1)

        out = self.linear1(embeds)
        out = self.activation(out)
        out = self.linear2(out)

        log_probs = torch.log_softmax(out, dim=1)
        return log_probs
