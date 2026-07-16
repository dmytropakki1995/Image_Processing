import torch
import torch.nn.functional as F

import pandas as pd

from openai import OpenAI
import numpy as np

import os
from dotenv import load_dotenv


################


load_dotenv()

EMBED_MODEL_SMALL = "text-embedding-3-small"  # 1536 вимірів
EMBED_MODEL_LARGE = "text-embedding-3-large"  # 3072 вимірів
LLM_MODEL = "gpt-4o-mini"  # для генерації відповідей
API_KEY = os.getenv("API_KEY")

client = OpenAI(api_key=API_KEY)

words = [
    "vision",
    "color",
    "red",
    "orange",
    "yellow",
    "green",
    "blue",
    "violet",
    "purple",
    "lilac",
    "taste",
    "bitter",
    "sweet",
    "sour"
]


def coordinate_intersections(B: torch.Tensor) -> list[list[list[int]]]:
    """Return the filtered embedding-coordinate indices shared by each row pair.

    ``B`` is expected to be a two-dimensional binary tensor with one row per
    word and one column per embedding coordinate.  The returned matrix has the
    same word-by-word layout as ``B @ B.T``.  Coordinate indices are zero-based.
    """
    if B.ndim != 2:
        raise ValueError("B must be a two-dimensional tensor")

    filtered = B.bool()
    return [
        [
            torch.nonzero(filtered[i] & filtered[j], as_tuple=True)[0].tolist()
            for j in range(filtered.shape[0])
        ]
        for i in range(filtered.shape[0])
    ]

def embed(text: str, model: str) -> np.ndarray:
    model_name = f"text-embedding-3-{model}"
    response = client.embeddings.create(model=model_name, input=text)
    return torch.tensor(response.data[0].embedding, dtype=torch.float32)
    # return np.array(response.data[0].embedding)

# cosine similarity
def cosine_sim(a, b):
    return np.dot(a, b) / (np.linalg.norm(a) * np.linalg.norm(b))


################


embeddings = {
    "small": torch.zeros(len(words), 1536),
    "large": torch.zeros(len(words), 3072),
}

for m in ["small", "large"]:
    for i in range(len(words)):
        embeddings[m][i] = embed(words[i], m)


################


VERSION = "4"
PATH  = f"model_artifacts/version_{VERSION}/"
os.makedirs(PATH, exist_ok=True)


################


# Sigmoid

T_s = {
    "large": [0.055, 0.065], # [0.05, 0.055, 0.065, 0.07]
    "small": [0.07, 0.075, 0.08, 0.85] # [0.07, 0.075, 0.08, 0.085, 0.09]
}

try:
    for model in T_s:
        # data = []
        
        for t in T_s[model]:
            
            print(t, " -> ", model)
            
            # filtering
            B = (embeddings[model] >= t).int()
    
            C = B @ B.T

            model_path = PATH + f"{model}/"
            os.makedirs(model_path, exist_ok=True)

            # ### Integral matrices
            # ###
            # data.append(C.tolist())
            # result = [
            #     [list(values) for values in zip(*rows)]
            #     for rows in zip(*data)
            # ]
            # d_values = f"{T_s[MODEL][0]}-{T_s[MODEL][-1]}" 
            # ###

            
            df = pd.DataFrame(C, index=words, columns=words)
            df.to_csv(f"{model_path}/filtration_s_{t}.csv", mode="w")

            intersections = coordinate_intersections(B)
            intersections_df = pd.DataFrame(
                intersections, index=words, columns=words
            )
            intersections_df.to_csv(
                f"{model_path}/coordinate_intersections_s_{t}.csv", mode="w"
            )
            
except Exception as e:
    print(f"An error occurred: {e}")


################


# Gauss
T_g = {
    "large": [0.0001, 0.0003], # [0.05, 0.055, 0.065, 0.07]
    "small": [0.0007, 0.001] # [0.07, 0.075, 0.08, 0.085, 0.09]
}

try:
    # data = []
    for model in T_g:
        for t in T_g[model]:
            print(t, " -> ", model)
            
            # filtering
            B = (torch.abs(embeddings[model]) <= t).int()
    
            C = B @ B.T

            model_path = PATH + f"{model}/"
            os.makedirs(model_path, exist_ok=True)
        
            df = pd.DataFrame(C, index=words, columns=words)
            df.to_csv(f"{model_path}/filtration_g_{t}.csv", mode="w")

            intersections = coordinate_intersections(B)
            intersections_df = pd.DataFrame(
                intersections, index=words, columns=words
            )
            intersections_df.to_csv(
                f"{model_path}/coordinate_intersections_g_{t}.csv", mode="w"
            )
            
except Exception as e:
    print(f"An error occurred: {e}")
