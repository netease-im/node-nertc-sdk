#include <assert.h>
#include <windows.h>

#include <algorithm>

#include "../../../base/string_utils.h"
#include "ring_buffer_mapping_file.h"

namespace nelog {
namespace {
size_t inline RemainingSpace(const size_t& capacity,
                             const size_t& index_writing,
                             const size_t& index_reading) {
  return index_reading <= index_writing
             ? capacity - index_writing + index_reading - 1
             : index_reading - index_writing - 1;
}

size_t inline DataLength(const size_t& capacity,
                         const size_t& index_writing,
                         const size_t& index_reading) {
  return index_writing >= index_reading
             ? index_writing - index_reading
             : capacity - index_reading + index_writing;
}
}  // namespace

struct RingBufferMappingFile::Label {
  char built_date[sizeof(__DATE__)] = __DATE__;
  char built_time[sizeof(__TIME__)] = __TIME__;
  size_t ring_buffer_capacity = 0;
  intptr_t index_writing = 0;
  intptr_t index_reading = 0;
  HANDLE file_handle = INVALID_HANDLE_VALUE;
  HANDLE file_mapping_handle = INVALID_HANDLE_VALUE;
  void* buffer = nullptr;
};

RingBufferMappingFile::RingBufferMappingFile() {}

RingBufferMappingFile::~RingBufferMappingFile() {
  Close();
}

bool RingBufferMappingFile::Open(size_t linear_buffer_capacity,
                                 size_t ring_buffer_capacity,
                                 const std::string& mmap_file_path) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (ring_buffer_capacity == 0 || mmap_file_path.empty() ||
      label_ != nullptr) {
    return false;
  }

  HANDLE file_mapping_handle = INVALID_HANDLE_VALUE;
  auto w_mmap_file_path = ToUtf16(mmap_file_path);
  HANDLE file_handle = CreateFileW(
      w_mmap_file_path.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ,
      NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

  if (file_handle == NULL || file_handle == INVALID_HANDLE_VALUE) {
    goto FAIL;
  }

  if (INVALID_SET_FILE_POINTER ==
      SetFilePointer(file_handle,
                     static_cast<LONG>(ring_buffer_capacity +
                                       linear_buffer_capacity + sizeof(Label)),
                     NULL, 0)) {
    goto FAIL;
  }

  if (0 == SetEndOfFile(file_handle)) {
    goto FAIL;
  }

  file_mapping_handle = CreateFileMapping(
      file_handle, nullptr, PAGE_READWRITE, 0,
      static_cast<DWORD>(ring_buffer_capacity + linear_buffer_capacity +
                         sizeof(Label)),
      NULL);
  if (file_mapping_handle == NULL ||
      file_mapping_handle == INVALID_HANDLE_VALUE) {
    goto FAIL;
  }

  label_ = reinterpret_cast<Label*>(MapViewOfFile(
      file_mapping_handle, FILE_MAP_ALL_ACCESS, 0, 0,
      ring_buffer_capacity + linear_buffer_capacity + sizeof(Label)));
  if (label_ == nullptr) {
    goto FAIL;
  }

  if (strncmp(__DATE__, label_->built_date, sizeof(label_->built_date)) != 0 ||
      strncmp(__TIME__, label_->built_time, sizeof(label_->built_time)) != 0 ||
      label_->ring_buffer_capacity != ring_buffer_capacity ||
      (size_t)label_->index_writing > label_->ring_buffer_capacity ||
      (size_t)label_->index_reading > label_->ring_buffer_capacity) {
    *label_ = Label();
    label_->index_reading = 0;
    label_->index_writing = 0;
  }

  label_->ring_buffer_capacity = ring_buffer_capacity;
  label_->buffer = (uint8_t*)label_ + sizeof(Label) + linear_buffer_capacity;
  label_->file_handle = file_handle;
  label_->file_mapping_handle = file_mapping_handle;
  label_->index_writing %= ring_buffer_capacity;
  label_->index_reading %= ring_buffer_capacity;
  linear_buffer_ = label_ + 1;
  read_blocking_ = true;
  return true;
FAIL:
  if (file_mapping_handle != NULL &&
      file_mapping_handle != INVALID_HANDLE_VALUE) {
    CloseHandle(file_mapping_handle);
  }
  if (file_handle != NULL && file_handle != INVALID_HANDLE_VALUE) {
    CloseHandle(file_handle);
  }
  return false;
}

void RingBufferMappingFile::Close() {
  SetReadBlocking(false);
  std::unique_lock<std::mutex> lock(mutex_);
  if (label_ != nullptr) {
    HANDLE file_mapping_handle = label_->file_mapping_handle;
    HANDLE file_handle = label_->file_handle;
    UnmapViewOfFile(label_);
    if (file_mapping_handle != NULL &&
        file_mapping_handle != INVALID_HANDLE_VALUE) {
      CloseHandle(file_mapping_handle);
    }
    if (file_handle != NULL && file_handle != INVALID_HANDLE_VALUE) {
      CloseHandle(file_handle);
    }
  }
  label_ = nullptr;
  linear_buffer_ = nullptr;
}

size_t RingBufferMappingFile::Write(const char* data, size_t size) {
  if (data == nullptr || size == 0) {
    return 0;
  }
  std::unique_lock<std::mutex> lock(mutex_);
  if (label_ == nullptr) {
    return 0;
  }
  size = std::min<size_t>(
      size, RemainingSpace(label_->ring_buffer_capacity, label_->index_writing,
                           label_->index_reading));
  if (size == 0) {
    return 0;
  }

  size_t written_size = 0;
  while (written_size != size) {
    size_t written_size_once =
        std::min<size_t>(size - written_size,
                         label_->ring_buffer_capacity - label_->index_writing);
    memcpy(reinterpret_cast<char*>(label_->buffer) + label_->index_writing,
           data + written_size, written_size_once);
    written_size += written_size_once;
    label_->index_writing = (label_->index_writing + written_size_once) %
                            label_->ring_buffer_capacity;
  }
  condition_.notify_all();
  return written_size;
}

size_t RingBufferMappingFile::Read(
    size_t size,
    const std::function<void(const char*, size_t)>& handle,
    int64_t timeout) {
  if (size == 0 || handle == nullptr) {
    return 0;
  }

  std::unique_lock<std::mutex> lock(mutex_);
  if (label_ == nullptr) {
    return 0;
  }

  size = std::min<size_t>(size, label_->ring_buffer_capacity - 1);
  condition_.wait_for(lock, std::chrono::milliseconds(timeout), [this, size] {
    return (!read_blocking_ ||
            DataLength(label_->ring_buffer_capacity, label_->index_writing,
                       label_->index_reading) >= size);
  });

  size = std::min<size_t>(
      size, DataLength(label_->ring_buffer_capacity, label_->index_writing,
                       label_->index_reading));
  if (size == 0) {
    return 0;
  }
  size_t read_size = 0;
  while (read_size != size) {
    size_t read_size_once = std::min<size_t>(
        size - read_size, label_->ring_buffer_capacity - label_->index_reading);
    lock.unlock();
    handle(reinterpret_cast<char*>(label_->buffer) + label_->index_reading,
           read_size_once);
    lock.lock();
    read_size += read_size_once;
    label_->index_reading =
        (label_->index_reading + read_size_once) % label_->ring_buffer_capacity;
  }
  return read_size;
}

void RingBufferMappingFile::SetReadBlocking(const bool blocking) {
  std::unique_lock<std::mutex> lock(mutex_);
  read_blocking_ = blocking;
  condition_.notify_all();
}

void* RingBufferMappingFile::GetLinearBuffer() {
  std::unique_lock<std::mutex> lock(mutex_);
  return linear_buffer_;
}

bool RingBufferMappingFile::IsEmpty() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (label_ == nullptr) {
    return false;
  }

  return label_->index_writing == label_->index_reading;
}
}  // namespace nelog