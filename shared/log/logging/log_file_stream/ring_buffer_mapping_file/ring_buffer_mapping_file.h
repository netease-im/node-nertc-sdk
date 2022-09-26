#pragma once

#include <cstddef>
#include <mutex>
#include <string>
#include <functional>

namespace nelog {
class RingBufferMappingFile {
 public:
  RingBufferMappingFile();
  ~RingBufferMappingFile();
  bool Open(size_t linear_buffer_capacity, size_t ring_buffer_capacity,
            const std::string& mmap_file_path);
  void Close();
  size_t Write(const char* data, size_t size);
  size_t Read(size_t size,
              const std::function<void(const char*, size_t)>& handle,
              int64_t timeout);
  void SetReadBlocking(const bool blocking);
  void* GetLinearBuffer();
  bool IsEmpty();

 private:
  struct Label;
  bool read_blocking_ = true;
  std::mutex mutex_;
  std::condition_variable condition_;
  Label* label_ = nullptr;
  void* linear_buffer_ = nullptr;
};
}  // namespace nelog