terraform {
  backend "gcs" {
    bucket = "terraform-state-1f656"
    prefix = ""
  }
}
