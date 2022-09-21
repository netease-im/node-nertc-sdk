#pragma once

#include <string>

std::string ToHexString(uint8_t* data, size_t length);

#if defined(_WIN32) || defined(_WIN64)
std::wstring ToUtf16(const std::string& str);
std::string ToUtf8(const std::wstring& wstr);
#endif

inline bool StartsWith(const std::string& text,
                       const std::string& prefix) noexcept {
  return prefix.empty() ||
         (text.size() >= prefix.size() &&
          memcmp(text.data(), prefix.data(), prefix.size()) == 0);
}

inline bool EndsWith(const std::string& text,
                     const std::string& suffix) noexcept {
  return suffix.empty() || (text.size() >= suffix.size() &&
                            memcmp(text.data() + (text.size() - suffix.size()),
                                   suffix.data(), suffix.size()) == 0);
}
