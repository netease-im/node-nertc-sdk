#include "file_utils.h"

#include <cstring>
#include "string_utils.h"

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>

bool IsFile(const std::string& file) {
  WIN32_FILE_ATTRIBUTE_DATA data = {0};
  if (::GetFileAttributesExW(ToUtf16(file).c_str(), GetFileExInfoStandard,
                             &data) != 0) {
    return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0;
  }

  return false;
}

bool IsFolder(const std::string& file) {
  WIN32_FILE_ATTRIBUTE_DATA data = {0};
  if (::GetFileAttributesExW(ToUtf16(file).c_str(), GetFileExInfoStandard,
                             &data) != 0) {
    return (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ==
           FILE_ATTRIBUTE_DIRECTORY;
  }

  return false;
}

size_t GetFileSize(const std::string& file) {
  WIN32_FILE_ATTRIBUTE_DATA data = {0};
  if (::GetFileAttributesExW(ToUtf16(file).c_str(), GetFileExInfoStandard,
                             &data) != 0) {
    return data.nFileSizeLow;
  }

  return 0;
}

std::string AddTrailingPathDelimiterIfNeeded(const std::string& directory) {
  if (EndsWith(directory, "\\")) {
    return directory;
  }
  return directory + "\\";
}

std::vector<std::string> ListFiles(const std::string& directory) {
  std::vector<std::string> result;
  HANDLE handle;
  WIN32_FIND_DATAW find_data;
  LARGE_INTEGER size;
  handle = FindFirstFileW(ToUtf16(std::string(directory) + "\\*").c_str(),
                          &find_data);
  if (handle == INVALID_HANDLE_VALUE) {
    return result;
  }
  while (FindNextFileW(handle, &find_data)) {
    if (strcmp(ToUtf8(find_data.cFileName).c_str(), ".") == 0 ||
        strcmp(ToUtf8(find_data.cFileName).c_str(), "..") == 0) {
      continue;
    }

    auto absolute_path = AddTrailingPathDelimiterIfNeeded(directory) +
                         ToUtf8(find_data.cFileName);
    if (IsFolder(absolute_path)) {
      auto sub_files = ListFiles(absolute_path);
      result.insert(result.end(), sub_files.begin(), sub_files.end());
    } else if (IsFile(absolute_path)) {
      result.emplace_back(absolute_path);
    }
  }
  FindClose(handle);
  return result;
}

bool CreateDirectories(const std::string& directory) {
  auto path = AddTrailingPathDelimiterIfNeeded(directory);
  for (size_t i = 0; i < path.size(); i++) {
    if (path[i] == '\\' || path[i] == '/') {
      auto temp = path.substr(0, i + 1);
      if (IsFolder(temp)) {
        continue;
      } else if (IsFile(temp)) {
        return false;
      } else {
        if (!CreateDirectoryW(ToUtf16(temp).c_str(), nullptr)) {
          return false;
        }
      }
    }
  }
  return true;
}

#else
#include <dirent.h>
#include <sys/stat.h>

bool IsFile(const std::string& file) {
  struct stat st {};
  int res = ::stat(file.c_str(), &st);
  // Treat symlinks, named pipes, etc. all as files.
  return res == 0 && !S_ISDIR(st.st_mode);
}

bool IsFolder(const std::string& file) {
  struct stat st {};
  int res = ::stat(file.c_str(), &st);
  return res == 0 && S_ISDIR(st.st_mode);
}

size_t GetFileSize(const std::string& file) {
  struct stat st {};
  if (::stat(file.c_str(), &st) != 0) {
    return 0;
  }
  return st.st_size;
}

std::string AddTrailingPathDelimiterIfNeeded(const std::string& directory) {
  if (EndsWith(directory, "/")) {
    return directory;
  }
  return directory + "/";
}

std::vector<std::string> ListFiles(const std::string& directory) {
  std::vector<std::string> result;
  DIR* dir = ::opendir(directory.c_str());
  if (dir == nullptr) {
    return {};
  }

  for (struct dirent* dirent = ::readdir(dir); dirent;
       dirent = ::readdir(dir)) {
    if (strcmp(dirent->d_name, ".") == 0 || strcmp(dirent->d_name, "..") == 0) {
      continue;
    }

    auto absolute_path =
        AddTrailingPathDelimiterIfNeeded(directory) + dirent->d_name;
    if (IsFolder(absolute_path)) {
      auto sub_files = ListFiles(absolute_path);
      result.insert(result.end(), sub_files.begin(), sub_files.end());
    } else if (IsFile(absolute_path)) {
      result.emplace_back(absolute_path);
    }
  }
  ::closedir(dir);
  return result;
}

bool CreateDirectories(const std::string& directory) {
  auto path = AddTrailingPathDelimiterIfNeeded(directory);
  for (size_t i = 0; i < path.size(); i++) {
    if (path[i] == '/') {
      auto temp = path.substr(0, i + 1);
      if (IsFolder(temp)) {
        continue;
      } else if (IsFile(temp)) {
        return false;
      } else {
        if (mkdir(temp.c_str(), S_IRWXU | S_IRWXG | S_IRWXO) != 0) {
          return false;
        }
      }
    }
  }
  return true;
}
#endif
