#pragma once

#include <stddef.h>

#include <condition_variable>
#include <cstdio>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include "../../base/file/file.h"

namespace nelog {

class RingBufferMappingFile;

class LogFileStream {
 public:
  LogFileStream(const std::string& directory_path,
                const std::string& log_name_prefix,
                bool enable_encrypting_and_compressing);
  ~LogFileStream();
  bool Open();
  void Write(std::string& message);
  void Flush();
  std::string GetDirectoryPath();
  std::vector<std::string> GetFileList();

 private:
  LogFileStream(const LogFileStream&) = delete;
  LogFileStream& operator=(const LogFileStream&) = delete;
  void WriteToFile(const void* const data, size_t length);
  void WriteToFileAndRotateIfNeeded(const void* const data, size_t length);
  bool OpenCurrentFile(bool first = false);
  void CloseCurrentFile();
  void RotateFiles();

  std::mutex mutex_;
  std::condition_variable condition_variable_;
  // Convenience storage for file names so we don't generate them over and over.
  std::vector<std::string> file_names_;
  const std::string directory_path_;
  const std::string log_name_prefix_;
  const size_t max_file_size_ = 0;
  // The rotation index indicates the index of the file that will be
  // deleted first on rotation. Indices lower than this index will be rotated.
  const size_t rotation_index_ = 0;
  // Number of bytes written to current file. We need this because with
  // buffering the file size read from disk might not be accurate.
  size_t current_bytes_written_ = 0;
  File file_;
  std::shared_ptr<RingBufferMappingFile> ring_buffer_;
  std::unique_ptr<std::thread> thread_writing_file_;
  bool keep_alive_ = true;
  bool flushing_ = false;
};

}  // namespace nelog
