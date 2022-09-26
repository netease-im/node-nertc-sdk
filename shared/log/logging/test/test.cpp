#include "../logging.h"
#include <iostream>
#include <chrono>
#include <thread>

int main() {
#if defined(_WIN32) || defined(_WIN64)
  std::string log_directory("D:\\temp\\ne_log\\1\\中文\\2");
#else
  std::string log_directory("/Users/huanghuaicheng01/temp/ne_log/ne_log");
#endif
  auto error_code = nelog::InitailizeLogFileStream(log_directory.c_str(),
                                 "my_log", false);
  nelog::SetLogToStdout(true);
  nelog::SetMinLoggingSeverity(nelog::LS_INFO);
  nelog::LogFileAndLine(true);
  LOG(INFO) << "InitailizeLogFileStream: " << error_code;
  LOG(INFO) << "main.cpp";
  nelog::TerminateLogFileStream();
  return 0;
}
