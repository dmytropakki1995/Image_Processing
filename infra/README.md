### 🧰 Requirements
- [terraform](https://docs.astral.sh/uv/getting-started/installation/) with version 1.10 or later
- [SOPS](https://github.com/getsops/sops) to encrypt and decrypt sensitive data.


To run terraform code:
Make sure that you have access to GCS bucket with terraform state.

Run the folowing commands:
```sh
# init terraform 
terraform init 

# check any changes in the infrastructure
terraform plan -lock=false 

# apply changes
terraform apply 
```

To decrypt SA key with SOPS:
```sh
sops -d sa-key.enc.json > sa-key.json
```


