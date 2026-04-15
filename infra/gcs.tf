resource "google_storage_bucket" "cbow_model_data" {
  name          = "cbow-model-data-1f656"
  location      = var.region
  project       = var.project
  storage_class = "ARCHIVE"

  uniform_bucket_level_access = true
  force_destroy               = false

  versioning {
    enabled = false
  }
}

resource "google_storage_bucket" "cbow_training_data" {
  name          = "cbow-training-data-1f656"
  location      = var.region
  project       = var.project
  storage_class = "ARCHIVE"

  uniform_bucket_level_access = true
  force_destroy               = false

  versioning {
    enabled = false
  }
}
