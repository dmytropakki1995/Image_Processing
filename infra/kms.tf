resource "google_kms_key_ring" "sops" {
  name     = "sops-keyring"
  location = var.region
  project  = var.project
}

resource "google_kms_crypto_key" "sops" {
  name            = "sops-key"
  key_ring        = google_kms_key_ring.sops.id
  rotation_period = "2592000s" # 30 days
}