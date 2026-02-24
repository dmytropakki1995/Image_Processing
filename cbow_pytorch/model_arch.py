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



class CBOWModel(nn.Module):
    def __init__(self, vocab_size, embedding_dim):
        super().__init__()

        self.embedding = nn.Embedding(
            num_embeddings=vocab_size,
            embedding_dim=embedding_dim
        )

        self.fc = nn.Linear(embedding_dim, vocab_size)

    def forward(self, x):
        """
        x shape: (batch_size, 2 * WINDOW_SIZE)
        """
        # Embedding lookup
        embeds = self.embedding(x)  
        # embeds shape: (batch_size, 2 * WINDOW_SIZE, embedding_dim)

        # Mean over context words (equivalent to tf.reduce_mean(axis=1))
        mean_embeds = embeds.mean(dim=1)
        # shape: (batch_size, embedding_dim)

        # Linear layer
        out = self.fc(mean_embeds)
        # shape: (batch_size, vocab_size)

        return out


# def custom_standardization(text: str) -> str:
#     text = text.lower()
#     translator = str.maketrans('', '', string.punctuation)
#     return text.translate(translator)

# with open(DATA, "r", encoding="utf-8") as f:
#     input_data = f.read()
# data = custom_standardization(input_data)

# tokens = data.split()
# print("Total number of tokens:", len(tokens))

# # Count word frequencies
# word_counts = Counter(tokens)

# # Assign indices starting from 1 (same as Keras)
# vocab = {word: idx for idx, (word, _) in enumerate(word_counts.items(), start=1)}

# # Reverse mapping
# word2idx = {word: idx for idx, (word, _) in enumerate(vocab.items())}
# idx2word = {idx: word for word, idx in word2idx.items()}
# vocab_size = len(word2idx)
# print("Vocab size is:", vocab_size)

# sequence = torch.tensor(
#     [word2idx[word] for word in tokens],
#     dtype=torch.long
# )