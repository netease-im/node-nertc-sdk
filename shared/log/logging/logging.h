#pragma once

#include <inttypes.h>
#include <cassert>
#include <cstdarg>
#include <iomanip>
#include <sstream>
#include <type_traits>

#if defined(NE_LOG_EXOPRT_API)
#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef __GNUC__
#define NE_LOG_API __attribute__((dllexport))
#else
#define NE_LOG_API __declspec(dllexport)
#endif
#else
#if __GNUC__ >= 4
#define NE_LOG_API __attribute__((visibility("default")))
#else
#define NE_LOG_API
#endif
#endif
#else
#define NE_LOG_API
#endif

#define LOG(severity)                                                          \
  (nelog::LoggingSeverity::LS_##severity >= nelog::GetMinLoggingSeverity()) && \
      nelog::internal::LogMessage(                                             \
          nelog::LoggingSeverity::LS_##severity,                               \
          &__FILE__[std::integral_constant<size_t,                             \
                                           GetFileNameOffset(__FILE__)>()],    \
          __FUNCTION__, __LINE__) &                                            \
          std::ostringstream()

#define LOG_F(severity, format, ...)                                           \
  (nelog::LoggingSeverity::LS_##severity >= nelog::GetMinLoggingSeverity()) && \
      nelog::internal::LogMessage(                                             \
          nelog::LoggingSeverity::LS_##severity,                               \
          &__FILE__[std::integral_constant<size_t,                             \
                                           GetFileNameOffset(__FILE__)>()],    \
          __FUNCTION__, __LINE__)                                              \
          .LogFormat(format, ##__VA_ARGS__)

namespace {
template <typename T, size_t size>
inline constexpr size_t GetFileNameOffset(const T (&file_path)[size],
                                          size_t i = size - 1) {
  static_assert(size > 1, "");
  if (file_path[i] == '/' || file_path[i] == '\\') {
    return i + 1;
  } else if (i == 0) {
    return 0;
  } else {
    return GetFileNameOffset(file_path, i - 1);
  }
}

}  // namespace

namespace nelog {

enum LoggingSeverity : uint8_t {
  LS_SENSITIVE = 0,
  LS_VERBOSE = 1,
  LS_DETAIL_INFO = 2,
  LS_INFO = 3,
  LS_WARNING = 4,
  LS_ERROR = 5,
  LS_NONE = 6,
};

enum ErrorCode : uint32_t {
  OK = 0,
  UNKNOWN_ERROR = 1,
  UNINITIALIZED = 2,
  INVALID_STATE = 3,
  INVALID_PARAMETERS = 4,
};

class LogSink {
 public:
  virtual ~LogSink() = default;
  virtual void OnLogMessage(const char* const message) = 0;
};

NE_LOG_API ErrorCode
InitailizeLogFileStream(const char* directory_path,
                        const char* log_name_prefix,
                        bool enable_encrypting_and_compressing);

NE_LOG_API ErrorCode InitailizeLogFileStream(const char* directory_path,
                                             const char* log_name_prefix);

NE_LOG_API ErrorCode TerminateLogFileStream();

NE_LOG_API void SetMinLoggingSeverity(LoggingSeverity logging_severity);

NE_LOG_API LoggingSeverity GetMinLoggingSeverity();

NE_LOG_API void SetLogToStdout(bool enable = true);

NE_LOG_API void LogFileAndLine(bool enable = true);

NE_LOG_API void AddLogSink(LogSink* log_sink);

NE_LOG_API void RemoveLogSink(LogSink* log_sink);

NE_LOG_API ErrorCode Log(const char* message);

namespace internal {
class LogMessage final {
 public:
  NE_LOG_API LogMessage(LoggingSeverity logging_severity,
                        const char* const file,
                        const char* const function,
                        const uint32_t line);
  ~LogMessage() = default;

  inline bool operator&(std::ostringstream&& message) {
    Log(message.str().c_str());
    return true;
  }

  bool LogFormat(const char* format, ...) {
    assert(format != nullptr);
    va_list args, copy;
    va_start(args, format);
    va_copy(copy, args);
    const auto predicted_length = std::vsnprintf(nullptr, 0, format, copy);
    va_end(copy);

    std::string message;
    assert(predicted_length >= 0);
    if (predicted_length > 0) {
      message.resize(predicted_length);
      // Pass "+ 1" to vsnprintf to include space for the '\0'.
      const auto actual_length =
          std::vsnprintf(&message[0], predicted_length + 1, format, args);
      assert(actual_length >= 0);
    }
    va_end(args);
    Log(message.c_str());
    return true;
  }

 private:
  LogMessage() = delete;
  LogMessage(const LogMessage&) = delete;
  NE_LOG_API void Log(const char* message);

  const LoggingSeverity logging_severity_ = LS_NONE;
  const char* const file_ = nullptr;
  const char* const function_ = nullptr;
  const uint32_t line_ = 0;
};
}  // namespace internal
}  // namespace nelog
