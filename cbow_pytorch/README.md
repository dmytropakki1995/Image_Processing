This simple guide is used to run CBOW model using PyTorch.

### 🧰 Requirements
- **uv** version 0.9.5 or later [see the installation guide](https://docs.astral.sh/uv/getting-started/installation/)
- Dowload [model](https://drive.google.com/drive/folders/1pmf8ZEWMLsy7M4i6eB3-nZqaqymxFcNn) folder with pretrained weights into the current directory.

### 🛠 Run project with UV
```sh
## 1. Install venv
# Windows
uv venv && .venv\Scripts\activate
# Linux
uv venv && source .venv/bin/activate

## 2. Sync dependencies
uv sync

## 3. Run Jupyter Notebook in Browser
uv run python -m jupyter notebook
```