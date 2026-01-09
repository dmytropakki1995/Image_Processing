This simple guide is used to run CBOW model using PyTorch.

### 🧰 Requirements
- **uv** version 0.9.5 or later [see the installation guide](https://docs.astral.sh/uv/getting-started/installation/)

### 🛠 Run project with UV
```sh
## 1. Install venv
# Windows
uv venv && .venv\Scripts\activate
# Linux
uv venv && source .venv/bin/activate

## 2. Sync dependencies
uv sync

## 3. Dowload 'model' folder into the current directory from https://drive.google.com/drive/folders/1pmf8ZEWMLsy7M4i6eB3-nZqaqymxFcNn

## 4. Run Jupyter Notebook in Browser
uv run python -m jupyter notebook
```