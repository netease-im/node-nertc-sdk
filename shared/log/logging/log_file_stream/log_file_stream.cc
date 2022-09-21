#include "log_file_stream.h"

#include <algorithm>
#include <cstdio>
#include <string>
#include <utility>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include "../../base/string_utils.h"

#include "../../base/file_utils.h"
#include "ring_buffer_mapping_file/ring_buffer_mapping_file.h"

#define RING_BUFFER_MAPPING_FILE_SIZE (150 * 1024U)  // 150K
#define MAX_LOG_FILE_NUM (5)
#define MAX_RAW_LOG_FILE_SIZE (20 * 1024 * 1024)
#define LOG_FILE_PREFIX_DEFAULT "ne_log"

namespace nelog {

namespace {

// |dir| must have a trailing delimiter. |prefix| must not include wild card
// characters.
std::vector<std::string> GetFilesWithPrefix(const std::string& directory,
                                            const std::string& prefix);
bool DeleteFile(const std::string& file);
bool MoveFile(const std::string& old_file, const std::string& new_file);
bool IsFile(const std::string& file);
bool IsFolder(const std::string& file);
bool GetFileSize(const std::string& file, size_t& size);

#if defined(_WIN32) || defined(_WIN64)

std::vector<std::string> GetFilesWithPrefix(const std::string& directory,
                                            const std::string& prefix) {
  WIN32_FIND_DATAW data;
  HANDLE handle;
  handle = ::FindFirstFileW(ToUtf16(directory + prefix + '*').c_str(), &data);
  if (handle == INVALID_HANDLE_VALUE)
    return {};

  std::vector<std::string> file_list;
  do {
    file_list.emplace_back(directory + ToUtf8(data.cFileName));
  } while (::FindNextFileW(handle, &data) == TRUE);

  ::FindClose(handle);
  return file_list;
}

bool DeleteFile(const std::string& file) {
  return ::DeleteFileW(ToUtf16(file).c_str()) != 0;
}

bool MoveFile(const std::string& old_file, const std::string& new_file) {
  return ::MoveFileW(ToUtf16(old_file).c_str(), ToUtf16(new_file).c_str()) != 0;
}

bool IsFile(const std::string& file) {
  WIN32_FILE_ATTRIBUTE_DATA data = {0};
  if (0 == ::GetFileAttributesExW(ToUtf16(file).c_str(), GetFileExInfoStandard,
                                  &data))
    return false;
  return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

bool IsFolder(const std::string& file) {
  WIN32_FILE_ATTRIBUTE_DATA data = {0};
  if (0 == ::GetFileAttributesExW(ToUtf16(file).c_str(), GetFileExInfoStandard,
                                  &data))
    return false;
  return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ==
         FILE_ATTRIBUTE_DIRECTORY;
}

bool GetFileSize(const std::string& file, size_t& size) {
  WIN32_FILE_ATTRIBUTE_DATA data = {0};
  if (::GetFileAttributesExW(ToUtf16(file).c_str(), GetFileExInfoStandard,
                             &data) == 0) {
    return false;
  }
  size = data.nFileSizeLow;
  return true;
}

#else

std::vector<std::string> GetFilesWithPrefix(const std::string& directory,
                                            const std::string& prefix) {
  DIR* dir = ::opendir(directory.c_str());
  if (dir == nullptr)
    return {};
  std::vector<std::string> file_list;
  for (struct dirent* dirent = ::readdir(dir); dirent;
       dirent = ::readdir(dir)) {
    std::string name = dirent->d_name;
    if (name.compare(0, prefix.size(), prefix) == 0) {
      file_list.emplace_back(directory + name);
    }
  }
  ::closedir(dir);
  return file_list;
}

bool DeleteFile(const std::string& file) {
  return ::unlink(file.c_str()) == 0;
}

bool MoveFile(const std::string& old_file, const std::string& new_file) {
  return ::rename(old_file.c_str(), new_file.c_str()) == 0;
}

bool IsFile(const std::string& file) {
  struct stat st {};
  int res = ::stat(file.c_str(), &st);
  // Treat symlinks, named pipes, etc. all as files.
  return res == 0 && !S_ISDIR(st.st_mode);
}

bool IsFolder(const std::string& file) {
  struct stat st;
  int res = ::stat(file.c_str(), &st);
  return res == 0 && S_ISDIR(st.st_mode);
}

bool GetFileSize(const std::string& file, size_t& size) {
  struct stat st {};
  if (::stat(file.c_str(), &st) != 0) {
    return false;
  }
  size = st.st_size;
  return true;
}

#endif

template <typename T>
inline T Align(const T& value, const T& align_value) {
  return (value + align_value - 1) & ~(align_value - 1);
}

std::string GetFilePath(const std::string& directory_path,
                        const std::string& file_prefix,
                        const std::string& file_suffix,
                        size_t index,
                        size_t num_files) {
  const size_t buffer_size = 32;
  char file_postfix[buffer_size];
  // We want to zero pad the index so that it will sort nicely.
  const int max_digits = std::snprintf(nullptr, 0, "%zu", num_files - 1);
  //  RTC_DCHECK_LT(1 + max_digits, buffer_size);
  std::snprintf(file_postfix, buffer_size, "_%0*zu", max_digits, index);

  return directory_path + file_prefix + file_postfix + file_suffix;
}

}  // namespace

LogFileStream::LogFileStream(const std::string& directory_path,
                             const std::string& log_name_prefix,
                             bool enable_encrypting_and_compressing)
    : directory_path_(AddTrailingPathDelimiterIfNeeded(directory_path)),
      log_name_prefix_(log_name_prefix.empty() ? LOG_FILE_PREFIX_DEFAULT
                                               : log_name_prefix),
      max_file_size_(MAX_RAW_LOG_FILE_SIZE),
      rotation_index_(MAX_LOG_FILE_NUM - 1),
      current_bytes_written_(0) {
  // generate file names.
  const std::string file_suffix = ".log";

  for (size_t i = 0; i < MAX_LOG_FILE_NUM; ++i) {
    file_names_.push_back(GetFilePath(directory_path_, log_name_prefix_,
                                      file_suffix, i, MAX_LOG_FILE_NUM));
  }
}

LogFileStream::~LogFileStream() {
  keep_alive_ = false;
  if (ring_buffer_) {
    ring_buffer_->SetReadBlocking(false);
    thread_writing_file_->join();
  }

  CloseCurrentFile();
}

bool LogFileStream::Open() {
  // initailize ring buffer
  if (!CreateDirectories(directory_path_)) {
    return false;
  }
  ring_buffer_ = std::make_shared<RingBufferMappingFile>();
  const std::string mmap_file_name = "buffer_mapping_file.mmap0";
  const size_t linear_buffer_capacity = 0;
  if (!ring_buffer_->Open(
          linear_buffer_capacity, RING_BUFFER_MAPPING_FILE_SIZE,
          AddTrailingPathDelimiterIfNeeded(directory_path_) + mmap_file_name)) {
    ring_buffer_.reset();
    return false;
  }

  if (!OpenCurrentFile(true)) {
    ring_buffer_.reset();
    return false;
  }

  const uint64_t timeout = 1000;
  thread_writing_file_ = std::make_unique<std::thread>([this, timeout] {
    constexpr size_t read_size = RING_BUFFER_MAPPING_FILE_SIZE >> 2U;
    auto data_handle =
        std::bind(&LogFileStream::WriteToFileAndRotateIfNeeded, this,
                  std::placeholders::_1, std::placeholders::_2);
    while (ring_buffer_->Read(read_size, data_handle, timeout) > 0 ||
           keep_alive_) {
      std::unique_lock<std::mutex> lock(mutex_);
      if (flushing_) {
        flushing_ = false;
        condition_variable_.notify_one();
      }
    }
  });
  return true;
}

void LogFileStream::Write(std::string& message) {
  if (ring_buffer_) {
    ring_buffer_->Write(message.data(), message.length());
  }
}

void LogFileStream::Flush() {
  std::unique_lock<std::mutex> lock(mutex_);
  ring_buffer_->SetReadBlocking(false);
  flushing_ = true;
  condition_variable_.wait(lock, [this] { return !flushing_; });
  ring_buffer_->SetReadBlocking(true);
}

std::string LogFileStream::GetDirectoryPath() {
  return directory_path_;
}

std::vector<std::string> LogFileStream::GetFileList() {
  return file_names_;
}

void LogFileStream::WriteToFile(const void* const data, size_t length) {
  if (!IsFile(file_names_[0])) {
    OpenCurrentFile(false);
  }

  if (!file_.IsOpen()) {
    std::fprintf(stderr, "Open() must be called before Write.\n");
    return;
  }

  current_bytes_written_ +=
      file_.Write(reinterpret_cast<const uint8_t*>(data), length);
}

void LogFileStream::WriteToFileAndRotateIfNeeded(const void* const data,
                                                 size_t length) {
  WriteToFile(data, length);

  // If we're done with this file, rotate it out.
  if (current_bytes_written_ >= max_file_size_) {
    RotateFiles();
  }
}

bool LogFileStream::OpenCurrentFile(bool first) {
  CloseCurrentFile();
  current_bytes_written_ = 0;

  // Opens the appropriate file in the appropriate mode.
  const std::string& file_path = file_names_[0];
#if defined(_WIN32) || defined(_WIN64)
  file_ = IsFile(file_path)
              ? File::Open(file_path, FILE_SHARE_READ | FILE_SHARE_WRITE)
              : File::Create(file_path, FILE_SHARE_READ | FILE_SHARE_WRITE);
#else
  file_ = IsFile(file_path) ? File::Open(file_path) : File::Create(file_path);
#endif
  if (!file_.IsOpen()) {
    std::fprintf(stderr, "Failed to open: %s Error\n", file_path.c_str());
    return false;
  }

  if (!GetFileSize(file_path, current_bytes_written_)) {
    return false;
  }
  file_.Seek(current_bytes_written_);

  return true;
}

void LogFileStream::CloseCurrentFile() {
  file_.Close();
}

void LogFileStream::RotateFiles() {
  CloseCurrentFile();

  std::string file_to_delete = file_names_[rotation_index_];
  if (IsFile(file_to_delete)) {
    if (!DeleteFile(file_to_delete)) {
      std::fprintf(stderr, "Failed to delete: %s\n", file_to_delete.c_str());
    }
  }

  for (auto i = rotation_index_; i > 0; --i) {
    std::string rotated_name = file_names_[i];
    std::string unrotated_name = file_names_[i - 1];
    if (IsFile(unrotated_name)) {
      if (!MoveFile(unrotated_name, rotated_name)) {
        std::fprintf(stderr, "Failed to move: %s to %s\n",
                     unrotated_name.c_str(), rotated_name.c_str());
      }
    }
  }
  // Create a new file for 0th index.
  OpenCurrentFile();
}

}  // namespace nelog
