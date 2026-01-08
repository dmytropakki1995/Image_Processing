This simple guide is used to create **uv** project and install some Python dependencies.

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

## 3. Run Jupyter Notebook in Browser
uv run python -m jupyter notebook
```