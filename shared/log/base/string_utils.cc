#include "string_utils.h"

#include <iomanip>
#include <sstream>
#include <string>
#include <cstring>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

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

inline std::string ToUtf8(const wchar_t* wide, size_t len) {
  int len8 = ::WideCharToMultiByte(CP_UTF8, 0, wide, static_cast<int>(len),
                                   nullptr, 0, nullptr, nullptr);
  char* ns = STACK_ARRAY(char, len8);
  ::WideCharToMultiByte(CP_UTF8, 0, wide, static_cast<int>(len), ns, len8,
                        nullptr, nullptr);
  return std::string(ns, len8);
}

inline std::string ToUtf8(const wchar_t* wide) {
  return ToUtf8(wide, wcslen(wide));
}

std::string ToUtf8(const std::wstring& wstr) {
  return ToUtf8(wstr.data(), wstr.length());
}
#endif

std::string ToHexString(uint8_t* data, size_t length) {
  std::ostringstream result;
  for (size_t i = 0; i < length; i++) {
    result << std::setfill('0') << std::setw(2) << std::hex << (data[i] & 0xFF);
  }
  return result.str();
}