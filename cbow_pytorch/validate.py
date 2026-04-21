import os
import torch
import torch.nn.functional as F

import update_model
import model_arch

def load_embeddings_and_vocab(config_path: str = "./model/model_ns_config.json", model_dir: str = "./model"):
    """
    Loads the trained model and returns the input embeddings
    along with vocabulary mappings.
    """
    cfg = update_model.load_config(config_path)
    model = update_model.load_model(cfg, model_dir)
    model.eval()

    # Get input context embeddings based on architecture type
    if isinstance(model, model_arch.CBOWModelNS):
        embeddings = model.get_input_embeddings() # Shape: (vocab_size, embedding_dim)
    else:
        embeddings = model.embedding.weight.data

    # L2 Normalize the embeddings ahead of time for fast cosine similarity via dot product
    embeddings = F.normalize(embeddings, p=2, dim=1)

    word2idx = cfg["word2idx"]
    idx2word = {i: w for w, i in word2idx.items()}

    return embeddings, word2idx, idx2word


def find_similar_words(
    query_word: str,
    embeddings: torch.Tensor,
    word2idx: dict,
    idx2word: dict,
    k: int = 10
):
    """
    Finds the top-K most similar words to the query_word using cosine similarity.
    """
    if query_word not in word2idx:
        print(f" -- Word '{query_word}' not found in vocabulary.")
        return []

    query_idx = word2idx[query_word]
    query_vec = embeddings[query_idx].unsqueeze(0)  # (1, embedding_dim)

    # Cosine similarity (since embeddings are already L2 normalized, it's just a dot product)
    similarities = torch.matmul(embeddings, query_vec.T).squeeze()  # (vocab_size,)

    # Get top k+1 to exclude the query word itself from the results
    top_scores, top_indices = torch.topk(similarities, k=k + 1)
    
    results = []
    for score, idx in zip(top_scores.tolist(), top_indices.tolist()):
        if idx == query_idx:
            continue
        results.append((idx2word[idx], score))
        if len(results) == k:
            break

    return results


def word_analogy(
    word_a: str, word_b: str, word_c: str,
    embeddings: torch.Tensor,
    word2idx: dict,
    idx2word: dict,
    k: int = 5
):
    """
    Solves analogies of the form  A is to B as C is to ? 
    (e.g., man : king :: woman : queen)
    Returns top-K candidates for the missing word.
    """
    for w in [word_a, word_b, word_c]:
        if w not in word2idx:
            print(f" -- Word '{w}' not found in vocabulary.")
            return []

    vec_a = embeddings[word2idx[word_a]]
    vec_b = embeddings[word2idx[word_b]]
    vec_c = embeddings[word2idx[word_c]]

    # Formula: query = vec_b - vec_a + vec_c
    query_vec = vec_b - vec_a + vec_c
    query_vec_norm = F.normalize(query_vec, p=2, dim=0).unsqueeze(0)

    # Cosine similarities
    similarities = torch.matmul(embeddings, query_vec_norm.T).squeeze()
    
    # We fetch a few extra items because we want to filter out the query words
    top_scores, top_indices = torch.topk(similarities, k=k + 3)
    
    ignore_indices = {word2idx[word_a], word2idx[word_b], word2idx[word_c]}
    
    results = []
    for score, idx in zip(top_scores.tolist(), top_indices.tolist()):
        if idx in ignore_indices:
            continue
        results.append((idx2word[idx], score))
        if len(results) == k:
            break

    return results

if __name__ == "__main__":
    # Example usage if you run the script directly:
    # uv run python validate.py
    config_path = "./model/model_ns_config.json"
    model_dir = "./model"
    
    if os.path.exists(config_path):
        print("Loading embeddings...")
        embeddings, w2i, i2w = load_embeddings_and_vocab(config_path, model_dir)
        
        print("\n" + "="*40)
        test_word = "good"
        print(f"Top 10 similar words to '{test_word}':")
        res = find_similar_words(test_word, embeddings, w2i, i2w)
        for w, score in res:
            print(f"  {w:15} {score:.4f}")
        print("="*40)
    else:
        print(f"Could not load config from {config_path}. Train the model first.")
