#!/usr/bin/env python
# coding: utf-8

# In[1]:


import torch
import torch.nn as nn
from torch.nn import functional as F
torch.manual_seed(42)


# In[2]:


with open('input.txt', 'r', encoding='utf-8') as f:
    text = f.read()
print("length of dataset in characters: ", len(text))


# In[3]:


chars=" \n!$&',-.3:;?ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
stoi = { ch:i for i,ch in enumerate(chars) }
itos = { i:ch for i,ch in enumerate(chars) }
encode = lambda s: [stoi[c] for c in s] # encoder: take a string, output a list of integers
decode = lambda l: ''.join([itos[i] for i in l]) # decoder: take a list of integers, output a string

print(encode("llm is cool"))
# print(decode(encode("llm is cool")))


# In[4]:


data = torch.tensor(encode(text), dtype=torch.long)


# In[5]:


n = int(0.9*len(data)) # first 90% will be train, rest val
train_data = data[:n]
val_data = data[n:]


# In[6]:


block_size = 8
print(train_data[:block_size+1])
print(text[:block_size+1])


# In[7]:


batch_size = 4 # how many independent sequences will we process in parallel?
block_size = 8 # what is the maximum context length for predictions?


# In[8]:


def get_batch(split):
    # generate a small batch of data of inputs x and targets y
    data = train_data if split == 'train' else val_data
    ix = torch.randint(len(data) - block_size, (batch_size,))
    x = torch.stack([data[i:i+block_size] for i in ix])
    y = torch.stack([data[i+1:i+block_size+1] for i in ix])
    return x, y

xb, yb = get_batch('train')


# In[9]:


class BigramLanguageModel(nn.Module):
    def __init__(self, vocab_size):
        super().__init__()
        self.token_embedding_table = nn.Embedding(vocab_size, vocab_size)

    def forward(self, idx, targets=None):
        logits = self.token_embedding_table(idx)
        if targets is None:
            loss = None
        else:
            B, T, C = logits.shape # it will be [4, 8, 65] in our case
            logits = logits.view(B*T, C) # [32, 65]
            targets = targets.view(B*T) # [32]
            loss = F.cross_entropy(logits, targets) 

        return logits, loss
    def generate(self, idx, max_new_tokens):
        for _ in range(max_new_tokens):
            logits, loss = self(idx)
            logits = logits[:, -1, :] # берем последний степ для bigram предсказания
            probs = F.softmax(logits, dim=-1)
            idx_next = torch.multinomial(probs, num_samples = 1)
            idx = torch.cat((idx, idx_next), dim = 1)
        return idx


vocab_size = len(stoi)
m = BigramLanguageModel(vocab_size)
logits, loss = m(idx=xb, targets=yb)
# print(logits.shape, logits, loss)

idx = torch.zeros((1,1), dtype = torch.long)
# # принт ниже напринтит чушь, потому что модель не обучена
# m.generate(idx, max_new_tokens=100)[0].tolist()

print(decode(m.generate(idx, max_new_tokens=100)[0].tolist()))


# In[10]:


optimizer = torch.optim.AdamW(m.parameters(), lr=1e-3)


# In[11]:


batch_size = 32
for steps in range(100000): # increase number of steps for good results...
    # sample a batch of data
    xb, yb = get_batch('train')
    # evaluate the loss
    logits, loss = m(xb, yb)
    optimizer.zero_grad(set_to_none=True)
    loss.backward()
    optimizer.step()

print(loss.item())


# In[12]:


print(decode(m.generate(idx, max_new_tokens=100)[0].tolist()))


# In[13]:


B, T, C = 4, 8, 2
x = torch.randn(B, T, C)

xbow = torch.zeros((B,T,C)) # нули
for b in range(B): # итерируем по батчам
    for t in range(T): # итерируем по времени
        xprev = x[b,:t+1] # (t,C) 
        xbow[b,t] = torch.mean(xprev, 0)

wei = torch.tril(torch.ones(T, T))
wei = wei / wei.sum(1, keepdim=True)
xbow2 = wei @ x # (B, T, T) @ (B, T, C) ----> (B, T, C)
torch.allclose(xbow, xbow2)


# In[14]:


# Задаем размеры входных данных: 
# B - размер батча (количество примеров в одной итерации),
# T - временные шаги,
# C - количество каналов (размер признакового пространства или число признаков).
B, T, C = 4, 8, 32  # batch, time, channels

# Создаем случайные данные с нормальным распределением, имитируя входные последовательности размером BxTxC
x = torch.randn(B, T, C)

# Определяем размер головы внимания (self-attention). Это размер вектора, на который будем проецировать ключи, запросы и значения.
head_size = 16

# Создаем три линейных слоя для ключей (key), запросов (query) и значений (value).
# Они преобразуют входные данные (размер C) в векторы размером head_size.
key = nn.Linear(C, head_size, bias=False)
query = nn.Linear(C, head_size, bias=False)
value = nn.Linear(C, head_size, bias=False)

# Применяем линейное преобразование к входным данным, получая ключи и запросы для каждого временного шага.
k = key(x)   # (B, T, 16) - матрица ключей
q = query(x) # (B, T, 16) - матрица запросов

# Вычисляем матрицу весов внимания через матричное произведение запросов и транспонированных ключей.
# Это вычисляет сходство между запросами и ключами. Размерность: (B, T, T)
wei = q @ k.transpose(-2, -1)

# Создаем нижнетреугольную матрицу (T x T), которая используется для маскирования будущих временных шагов
# в последовательности (например, для авторегрессии), чтобы модель не "смотрела" вперед во времени.
tril = torch.tril(torch.ones(T, T))

# Применяем маскирование: все элементы выше главной диагонали заменяются на -inf, чтобы исключить их при вычислении softmax.
wei = wei.masked_fill(tril == 0, float('-inf'))

# Применяем softmax к весам, чтобы они стали вероятностями. Это делается по последнему измерению (T).
wei = F.softmax(wei, dim=-1)

# Применяем линейное преобразование к входным данным для получения значений.
v = value(x)

# Вычисляем выходной результат как взвешенное среднее значений с использованием весов внимания.
out = wei @ v  # Размер выхода: (B, T, 16)


# In[15]:


wei[0]


# In[16]:


class Head(nn.Module):
    """ one head of self-attention """

    def __init__(self, head_size):
        super().__init__()
        self.key = nn.Linear(n_embd, head_size, bias=False)
        self.query = nn.Linear(n_embd, head_size, bias=False)
        self.value = nn.Linear(n_embd, head_size, bias=False)
        self.register_buffer('tril', torch.tril(torch.ones(block_size, block_size)))

        self.dropout = nn.Dropout(dropout)

    def forward(self, x):
        B,T,C = x.shape
        k = self.key(x)   # (B,T,C)
        q = self.query(x) # (B,T,C)
        # compute attention scores ("affinities")
        wei = q @ k.transpose(-2,-1) * C**-0.5 # (B, T, C) @ (B, C, T) -> (B, T, T)
        wei = wei.masked_fill(self.tril[:T, :T] == 0, float('-inf')) # (B, T, T)
        wei = F.softmax(wei, dim=-1) # (B, T, T)
        wei = self.dropout(wei)
        # perform the weighted aggregation of the values
        v = self.value(x) # (B,T,C)
        out = wei @ v # (B, T, T) @ (B, T, C) -> (B, T, C)
        return out


# In[17]:


class MultiHeadAttention(nn.Module):
    """ multiple heads of self-attention in parallel """

    def __init__(self, num_heads, head_size):
        super().__init__()
        self.heads = nn.ModuleList([Head(head_size) for _ in range(num_heads)])
        self.proj = nn.Linear(n_embd, n_embd)
        self.dropout = nn.Dropout(dropout)

    def forward(self, x):
        out = torch.cat([h(x) for h in self.heads], dim=-1)
        out = self.dropout(self.proj(out))
        return out


# In[18]:


@torch.no_grad()
def estimate_loss(eval_iters):
    out = {}
    model.eval()
    for split in ['train', 'val']:
        losses = torch.zeros(eval_iters)
        for k in range(eval_iters):
            X, Y = get_batch(split)
            logits, loss = model(X, Y)
            losses[k] = loss.item()
        out[split] = losses.mean()
    model.train()
    return out


# In[19]:


torch.zeros([2, 2])


# In[20]:


device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
print(device)

eval_interval = 5
max_iters = 100000

model = BigramLanguageModel(vocab_size)
m = model.to(device)
# print the number of parameters in the model
print(sum(p.numel() for p in m.parameters())/1e6, 'M parameters')

# create a PyTorch optimizer
optimizer = torch.optim.AdamW(model.parameters(), lr=1e-3)

for iter in range(max_iters):

    # every once in a while evaluate the loss on train and val sets
    if iter % eval_interval == 0 or iter == max_iters - 1:
        losses = estimate_loss(eval_interval)
        print(f"step {iter}: train loss {losses['train']:.4f}, val loss {losses['val']:.4f}")

    # sample a batch of data
    xb, yb = get_batch('train')

    # evaluate the loss
    logits, loss = model(xb, yb)
    optimizer.zero_grad(set_to_none=True)
    loss.backward()
    optimizer.step()


# In[22]:


print(decode(m.generate(idx, max_new_tokens=1000)[0].tolist()))


# In[ ]:




