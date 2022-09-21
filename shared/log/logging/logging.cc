#include "logging.h"

#include <atomic>
#include <chrono>
#include <iostream>
#include <set>
#include <utility>

#if defined(__APPLE__)
#include <unistd.h>
#elif defined(__ANDROID__)
#include <android/log.h>
#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include "../base/synchronization/rw_lock_wrapper.h"
#include "log_file_stream/log_file_stream.h"

namespace nelog {

volatile LoggingSeverity g_logging_severity = LS_INFO;
volatile bool g_log_to_stdout = true;
volatile bool g_log_file_and_line = false;

namespace {
std::unique_ptr<RWLockWrapper> g_log_sink_set_rw_lock(
    RWLockWrapper::CreateRWLock());
std::set<LogSink*> g_log_sinks;

std::unique_ptr<RWLockWrapper> g_log_file_stream_rw_lock(
    RWLockWrapper::CreateRWLock());
std::unique_ptr<LogFileStream> g_log_file_stream;

uintmax_t GetProcessId() {
#if defined(__APPLE__) || defined(__ANDROID__) || defined(__linux__)
  return getpid();
#elif defined(_WIN32) || defined(_WIN64)
  return GetCurrentProcessId();
#else
  return 0;
#endif
}

uintmax_t GetThreadId() {
#if defined(__APPLE__)
  return pthread_mach_thread_np(pthread_self());
#elif defined(__ANDROID__)
  return gettid();
#elif defined(_WIN32) || defined(_WIN64)
  return GetCurrentThreadId();
#elif defined(__linux__)
  return syscall(__NR_gettid);
#else
  return 0;
#endif
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
  ReadLockScoped read_lock_scoped(*g_log_sink_set_rw_lock);
  for (auto& log_sink : g_log_sinks) {
    if (log_sink != nullptr) {
      log_sink->OnLogMessage(message);
    }
  }
}

void WriteToLogFileStream(std::string&& message) {
  ReadLockScoped read_lock_scoped(*g_log_file_stream_rw_lock);
  if (g_log_file_stream) {
    g_log_file_stream->Write(message);
  }
}
}  // namespace

NE_LOG_API ErrorCode
InitailizeLogFileStream(const char* directory_path,
                        const char* log_name_prefix,
                        bool enable_encrypting_and_compressing) {
  if (directory_path == nullptr) {
    return ErrorCode::INVALID_PARAMETERS;
  }
  WriteLockScoped lock(*g_log_file_stream_rw_lock);
  g_log_file_stream.reset();
  g_log_file_stream = std::make_unique<LogFileStream>(
      directory_path, log_name_prefix, enable_encrypting_and_compressing);
  if (g_log_file_stream->Open()) {
    return ErrorCode::OK;
  } else {
    g_log_file_stream.reset();
    return ErrorCode::UNKNOWN_ERROR;
  }
}

NE_LOG_API ErrorCode InitailizeLogFileStream(const char* directory_path,
                                             const char* log_name_prefix) {
  return InitailizeLogFileStream(directory_path, log_name_prefix, false);
}

NE_LOG_API ErrorCode TerminateLogFileStream() {
  WriteLockScoped lock(*g_log_file_stream_rw_lock);
  g_log_file_stream.reset();
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
  WriteLockScoped write_lock_scoped(*g_log_sink_set_rw_lock);
  g_log_sinks.insert(log_sink);
}

NE_LOG_API void RemoveLogSink(LogSink* log_sink) {
  WriteLockScoped write_lock_scoped(*g_log_sink_set_rw_lock);
  g_log_sinks.erase(log_sink);
}

NE_LOG_API ErrorCode Log(const char* message) {
  if (message == nullptr) {
    return ErrorCode::INVALID_PARAMETERS;
  }

  if (g_log_to_stdout) {
    std::cout << message;
  }

  WriteToLogFileStream(std::string(message));

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
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto tm = std::localtime(&time);
}

NE_LOG_API void LogMessage::Log(const char* message) {
  auto now = std::chrono::system_clock::now();
  auto time = std::chrono::system_clock::to_time_t(now);
  auto tm = std::localtime(&time);
  std::ostringstream string_stream;
  string_stream << std::setfill('0') << std::setw(4) << tm->tm_year + 1900
                << "-" << std::setfill('0') << std::setw(2) << tm->tm_mon + 1
                << "-" << std::setfill('0') << std::setw(2) << tm->tm_mday
                << " " << std::setfill('0') << std::setw(2) << tm->tm_hour
                << ":" << std::setfill('0') << std::setw(2) << tm->tm_min << ":"
                << std::setfill('0') << std::setw(2) << tm->tm_sec << "."
                << std::setfill('0') << std::setw(3)
                << std::chrono::duration_cast<std::chrono::milliseconds>(
                       now.time_since_epoch())
                           .count() %
                       1000
                << " [" << LoggingSeverityToString(logging_severity_) << "] ["
                << GetProcessId() << "-" << GetThreadId() << "] ";

  if (g_log_file_and_line) {
    string_stream << "(" << file_ << ":" << line_ << ") ";
  }

  string_stream << function_ << ": " << message << '\n';

  auto message_string = string_stream.str();
  if (g_log_to_stdout) {
    std::cout << message_string;
  }
  SendToSink(message_string.c_str());
  WriteToLogFileStream(std::move(message_string));
}

}  // namespace internal
}  // namespace nelog
