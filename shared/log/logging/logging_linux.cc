#include "logging.h"
#include <unistd.h>
#include <sys/syscall.h>

namespace nelog {

// Linux 平台空实现，避免链接问题
volatile LoggingSeverity g_logging_severity = LS_INFO;
volatile bool g_log_to_stdout = true;
volatile bool g_log_file_and_line = false;

namespace {
// 空变量，避免链接错误
volatile bool g_dummy = false;
}

uintmax_t GetProcessId() {
  return getpid();
}

uintmax_t GetThreadId() {
  return syscall(__NR_gettid);
}

const char* LoggingSeverityToString(const LoggingSeverity logging_severity) {
  switch (logging_severity) {
    case LoggingSeverity::LS_SENSITIVE:
      return "SENSITIVE";
    case LoggingSeverity::LS_VERBOSE:
      return "VERBOSE";
    case LoggingSeverity::LS_DETAIL_INFO:
      return "DETAIL_INFO";
    case LoggingSeverity::LS_INFO:
      return "INFO";
    case LoggingSeverity::LS_WARNING:
      return "WARNING";
    case LoggingSeverity::LS_ERROR:
      return "ERROR";
    case LoggingSeverity::LS_NONE:
      return "NONE";
    default:
      return "";
  }
}

void SendToSink(const char* const message) {
  // 空实现
}

void WriteToLogFileStream(std::string&& message) {
  // 空实现
}

NE_LOG_API ErrorCode
InitailizeLogFileStream(const char* directory_path,
                        const char* log_name_prefix,
                        bool enable_encrypting_and_compressing) {
  // Linux 平台返回成功，但不做任何实际操作
  return ErrorCode::OK;
}

NE_LOG_API ErrorCode InitailizeLogFileStream(const char* directory_path,
                                             const char* log_name_prefix) {
  return ErrorCode::OK;
}

NE_LOG_API ErrorCode TerminateLogFileStream() {
  return ErrorCode::OK;
}

NE_LOG_API void SetMinLoggingSeverity(LoggingSeverity logging_severity) {
  g_logging_severity = logging_severity;
}

NE_LOG_API LoggingSeverity GetMinLoggingSeverity() {
  return g_logging_severity;
}

NE_LOG_API void SetLogToStdout(bool enable) {
  g_log_to_stdout = enable;
}

NE_LOG_API void LogFileAndLine(bool enable) {
  g_log_file_and_line = enable;
}

NE_LOG_API void AddLogSink(LogSink* log_sink) {
  // 空实现
}

NE_LOG_API void RemoveLogSink(LogSink* log_sink) {
  // 空实现
}

NE_LOG_API ErrorCode Log(const char* message) {
  return ErrorCode::OK;
}

namespace internal {

NE_LOG_API LogMessage::LogMessage(LoggingSeverity logging_severity,
                                  const char* const file,
                                  const char* const function,
                                  const uint32_t line)
    : logging_severity_(logging_severity),
      file_(file),
      function_(function),
      line_(line) {
  // 空实现
}

NE_LOG_API void LogMessage::Log(const char* message) {
  // 空实现
}

}  // namespace internal

}  // namespace nelog
