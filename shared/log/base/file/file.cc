#include "file.h"

File::File(PlatformFile file) : file_(file) {}

File::File() : file_(kInvalidPlatformFileValue) {}

File::~File() {
  Close();
}

// static
File File::Open(const std::string& path, int shared_mode /* = 0*/) {
  return File(OpenPlatformFile(path, shared_mode));
}

// static
File File::Create(const std::string& path, int shared_mode /* = 0*/) {
  return File(CreatePlatformFile(path, shared_mode));
}

// static
bool File::Remove(const std::string& path) {
  return RemoveFile(path);
}

File::File(File&& other) : file_(other.file_) {
  other.file_ = kInvalidPlatformFileValue;
}

File& File::operator=(File&& other) {
  Close();
  file_ = other.file_;
  other.file_ = kInvalidPlatformFileValue;
  return *this;
}

bool File::IsOpen() {
  return file_ != kInvalidPlatformFileValue;
}
