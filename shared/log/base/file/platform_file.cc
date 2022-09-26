#include "platform_file.h"

#if defined(_WIN32) || defined(_WIN64)
#include <io.h>
#else
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

FILE* FdopenPlatformFileForWriting(PlatformFile file) {
  return FdopenPlatformFile(file, "w");
}

#if defined(_WIN32) || defined(_WIN64)
namespace {
#define STACK_ARRAY(TYPE, LEN) \
  static_cast<TYPE*>(::alloca((LEN) * sizeof(TYPE)))

std::wstring ToUtf16(const char* utf8, size_t len) {
  int len16 = ::MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(len),
                                    nullptr, 0);
  wchar_t* ws = STACK_ARRAY(wchar_t, len16);
  ::MultiByteToWideChar(CP_UTF8, 0, utf8, static_cast<int>(len), ws, len16);
  return std::wstring(ws, len16);
}

std::wstring ToUtf16(const std::string& str) {
  return ToUtf16(str.data(), str.length());
}
}  // namespace

const PlatformFile kInvalidPlatformFileValue = INVALID_HANDLE_VALUE;

FILE* FdopenPlatformFile(PlatformFile file, const char* modes) {
  if (file == kInvalidPlatformFileValue)
    return nullptr;
  int fd = _open_osfhandle(reinterpret_cast<intptr_t>(file), 0);
  if (fd < 0)
    return nullptr;

  return _fdopen(fd, modes);
}

bool ClosePlatformFile(PlatformFile file) {
  return CloseHandle(file) != 0;
}

bool RemoveFile(const std::string& path) {
  return ::DeleteFileW(ToUtf16(path).c_str()) != 0;
}

PlatformFile OpenPlatformFile(const std::string& path,
                              int shared_mode /*= 0*/) {
  return ::CreateFileW(ToUtf16(path).c_str(), GENERIC_READ | GENERIC_WRITE,
                       shared_mode, nullptr, OPEN_EXISTING,
                       FILE_ATTRIBUTE_NORMAL, nullptr);
}

PlatformFile OpenPlatformFileReadOnly(const std::string& path) {
  return ::CreateFileW(ToUtf16(path).c_str(), GENERIC_READ, FILE_SHARE_READ,
                       nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
}

PlatformFile CreatePlatformFile(const std::string& path,
                                int shared_mode /* = 0*/) {
  return ::CreateFileW(ToUtf16(path).c_str(), GENERIC_READ | GENERIC_WRITE,
                       shared_mode, nullptr, CREATE_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL, nullptr);
}

#else  // defined(_WIN32) || defined(_WIN64)

const PlatformFile kInvalidPlatformFileValue = -1;

FILE* FdopenPlatformFile(PlatformFile file, const char* modes) {
  return fdopen(file, modes);
}

bool ClosePlatformFile(PlatformFile file) {
  return close(file) == 0;
}

bool RemoveFile(const std::string& path) {
  return ::unlink(path.c_str()) == 0;
}

PlatformFile OpenPlatformFile(const std::string& path,
                              int shared_mode /*= 0*/) {
  return ::open(path.c_str(), O_RDWR);
}

PlatformFile OpenPlatformFileReadOnly(const std::string& path) {
  return ::open(path.c_str(), O_RDONLY);
}

PlatformFile CreatePlatformFile(const std::string& path,
                                int shared_mode /* = 0*/) {
  return ::open(path.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
}
#endif