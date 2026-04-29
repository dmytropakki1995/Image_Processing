# Service account configuration
resource "google_service_account" "gcs_sa" {
  account_id   = "gcs-sa"
  display_name = "Service account to get access to GCS bucket"
  project      = var.project
}

resource "google_service_account_key" "gcs_sa_key" {
  service_account_id = google_service_account.gcs_sa.name
}

# IAM roles
resource "google_storage_bucket_iam_member" "gcs_sa_model_data_access" {
  bucket = google_storage_bucket.cbow_model_data.name
  role   = "roles/storage.objectAdmin"
  member = format("serviceAccount:%s", google_service_account.gcs_sa.email)
}

resource "google_storage_bucket_iam_member" "gcs_sa_train_data_access" {
  bucket = google_storage_bucket.cbow_training_data.name
  role   = "roles/storage.objectAdmin"
  member = format("serviceAccount:%s", google_service_account.gcs_sa.email)
}
