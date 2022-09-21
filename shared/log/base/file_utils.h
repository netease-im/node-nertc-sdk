#pragma once

#include <string>
#include <vector>

bool IsFile(const std::string& file);

bool IsFolder(const std::string& file);

size_t GetFileSize(const std::string& file);

std::string AddTrailingPathDelimiterIfNeeded(const std::string& directory);

std::vector<std::string> ListFiles(const std::string& directory);

bool CreateDirectories(const std::string& directory);