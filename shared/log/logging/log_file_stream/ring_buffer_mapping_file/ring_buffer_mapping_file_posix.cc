#include <fcntl.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <unistd.h>

#include <algorithm>
#include <cassert>

#include "ring_buffer_mapping_file.h"

#if 0
#include <iostream>
#define LOG(x)                                                           \
  std::cout << "[" << this << ": " << __FUNCTION__ << "(): " << __LINE__ \
            << "] " << x << std::endl
#define LOGE(x)                                                          \
  std::cerr << "[" << this << ": " << __FUNCTION__ << "(): " << __LINE__ \
            << "] " << x << std::endl
#else
#define LOG(x) (void(0))
#define LOGE(x) (void(0))
#endif

namespace nelog {
namespace {
template <typename T>
inline T Align(T value, T align) {
  return (value + align - 1) & (~(align - 1));
}
}  // namespace

struct RingBufferMappingFile::Label {
  char built_date[sizeof(__DATE__)] = __DATE__;
  char built_time[sizeof(__TIME__)] = __TIME__;
  size_t ring_buffer_capacity = 0;
  intptr_t index_writing = 0;
  intptr_t index_reading = 0;
  int fd = -1;
  void* buffer = nullptr;
};

RingBufferMappingFile::RingBufferMappingFile() = default;

RingBufferMappingFile::~RingBufferMappingFile() { Close(); }

bool RingBufferMappingFile::Open(size_t linear_buffer_capacity,
                                 size_t ring_buffer_capacity,
                                 const std::string& mmap_file_path) {
  std::unique_lock<std::mutex> lock(mutex_);
  if (ring_buffer_capacity == 0 || mmap_file_path.empty() ||
      label_ != nullptr) {
    return false;
  }

  int ret = -1;
  void* buffer = nullptr;
  const size_t page_size = getpagesize();
  ring_buffer_capacity = Align(ring_buffer_capacity, page_size);
  assert(page_size >= sizeof(Label) + linear_buffer_capacity);
  label_ = reinterpret_cast<Label*>(
      mmap(nullptr,
           page_size + linear_buffer_capacity + (ring_buffer_capacity << 1U),
           PROT_NONE, MAP_ANONYMOUS | MAP_PRIVATE, -1, 0));
  LOG("label_:" << label_);

  if (label_ == MAP_FAILED || label_ == nullptr) {
    LOGE("error:" << strerror(errno));
    label_ = nullptr;
    return false;
  }

  int fd =
      open(mmap_file_path.c_str(), O_RDWR | O_CREAT | O_CLOEXEC | O_NONBLOCK,
           S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP);
  if (fd < 0) {
    LOGE("error:" << strerror(errno));
    goto FAIL;
  }

  ret = flock(fd, LOCK_NB | LOCK_EX);
  if (ret < 0) {
    LOGE("error:" << strerror(errno));
    goto FAIL;
  }

  ret = ftruncate(fd, ring_buffer_capacity + page_size);
  if (ret != 0) {
    LOGE("error:" << strerror(errno));
    goto FAIL;
  }

  lseek(fd, 0, SEEK_SET);

  buffer = mmap(label_, ring_buffer_capacity + page_size,
                PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED, fd, 0);
  LOG("buffer:" << buffer);
  if (buffer != label_ || access(mmap_file_path.c_str(), F_OK) == -1) {
    LOGE("error:" << strerror(errno));
    goto FAIL;
  }

  buffer =
      mmap(reinterpret_cast<char*>(label_) + ring_buffer_capacity + page_size,
           ring_buffer_capacity, PROT_READ | PROT_WRITE, MAP_FIXED | MAP_SHARED,
           fd, page_size);
  if (buffer !=
          reinterpret_cast<char*>(label_) + ring_buffer_capacity + page_size ||
      access(mmap_file_path.c_str(), F_OK) == -1) {
    LOGE("error:" << strerror(errno));
    goto FAIL;
  }

  if (strncmp(__DATE__, label_->built_date, sizeof(label_->built_date)) != 0 ||
      strncmp(__TIME__, label_->built_time, sizeof(label_->built_time)) != 0 ||
      label_->ring_buffer_capacity != ring_buffer_capacity ||
      label_->index_reading > label_->index_writing ||
      label_->index_writing > label_->ring_buffer_capacity << 1U) {
    LOG("reset label");
    memset(label_, '0', page_size);
    *(reinterpret_cast<char*>(label_) + page_size - 1) = '\n';
    *label_ = Label{.ring_buffer_capacity = ring_buffer_capacity};
  }

  label_->fd = fd;
  label_->buffer = reinterpret_cast<char*>(label_) + page_size;

  if (label_->index_reading >= label_->ring_buffer_capacity) {
    label_->index_reading -= label_->ring_buffer_capacity;
    label_->index_writing -= label_->ring_buffer_capacity;
  } else if (label_->index_writing - label_->index_reading >
             label_->ring_buffer_capacity) {
    label_->index_writing -= label_->ring_buffer_capacity;
  }

  linear_buffer_ = label_ + 1;
  read_blocking_ = true;
  LOG("open successfully");
  return true;

FAIL:
  if (fd >= 0) {
    close(fd);
  }

  if (label_ != MAP_FAILED && label_ != nullptr) {
    munmap(label_, page_size + (ring_buffer_capacity << 1U));
    label_ = nullptr;
  }

  return false;
}

void RingBufferMappingFile::Close() {
  SetReadBlocking(false);
  std::unique_lock<std::mutex> lock(mutex_);
  LOG("label_:" << label_);
  if (label_ != MAP_FAILED && label_ != nullptr) {
    if (label_->fd >= 0) {
      close(label_->fd);
      label_->fd = -1;
    }
    munmap(label_, getpagesize() + (label_->ring_buffer_capacity << 1U));
  }
  label_ = nullptr;
}

size_t RingBufferMappingFile::Write(const char* data, size_t size) {
  if (data == nullptr || size == 0) {
    return 0;
  }

  std::unique_lock<std::mutex> lock(mutex_);
  if (label_ == nullptr) {
    return 0;
  }
  if (label_->ring_buffer_capacity - label_->index_writing +
          label_->index_reading <
      size) {
    return 0;
  }
  memcpy(reinterpret_cast<char*>(label_->buffer) + label_->index_writing, data,
         size);
  label_->index_writing += size;
  condition_.notify_all();
  LOG("write index:" << label_->index_writing << ", read index:"
                     << label_->index_reading << ", remaining space:"
                     << (label_->capacity - label_->index_writing +
                         label_->index_reading)
                     << ", data length:"
                     << (label_->index_writing - label_->index_reading));
  return size;
}

size_t RingBufferMappingFile::Read(
    size_t size, const std::function<void(const char*, size_t)>& handle,
    int64_t timeout) {
  if (size == 0 || handle == nullptr) {
    return 0;
  }

  std::unique_lock<std::mutex> lock(mutex_);
  if (label_ == nullptr) {
    return 0;
  }
  condition_.wait_for(lock, std::chrono::milliseconds(timeout), [this, size] {
    return !read_blocking_ || (label_->index_writing - label_->index_reading >=
                               std::min(size, label_->ring_buffer_capacity));
  });

  size = std::min(
      static_cast<size_t>(label_->index_writing - label_->index_reading), size);
  lock.unlock();
  handle(reinterpret_cast<char*>(label_->buffer) + label_->index_reading, size);
  lock.lock();
  label_->index_reading += size;
  if (label_->index_reading >= label_->ring_buffer_capacity) {
    label_->index_reading -= label_->ring_buffer_capacity;
    label_->index_writing -= label_->ring_buffer_capacity;
  }

  condition_.notify_all();
  LOG("write index:" << label_->index_writing << ", read index:"
                     << label_->index_reading << ", remaining space:"
                     << (label_->capacity - label_->index_writing +
                         label_->index_reading)
                     << ", data length:"
                     << (label_->index_writing - label_->index_reading));
  return size;
}

void RingBufferMappingFile::SetReadBlocking(const bool blocking) {
  LOG("blocking:" << blocking);
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
