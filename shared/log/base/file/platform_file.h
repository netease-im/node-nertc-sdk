#pragma once

#include <stdio.h>
#include <string>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#if defined(_WIN32) || defined(_WIN64)
typedef HANDLE PlatformFile;
#else
typedef int PlatformFile;
#endif

extern const PlatformFile kInvalidPlatformFileValue;

// Associates a standard FILE stream with an existing PlatformFile.
// Note that after this function has returned a valid FILE stream,
// the PlatformFile should no longer be used.
FILE* FdopenPlatformFileForWriting(PlatformFile file);

// Associates a standard FILE stream with an existing PlatformFile.
// Note that after this function has returned a valid FILE stream,
// the PlatformFile should no longer be used.
FILE* FdopenPlatformFile(PlatformFile file, const char* modes);

// Closes a PlatformFile. Returns true on success, false on failure.
// Don't use ClosePlatformFile to close a file opened with FdopenPlatformFile.
// Use fclose instead.
bool ClosePlatformFile(PlatformFile file);

// Removes a file in the filesystem.
bool RemoveFile(const std::string& path);

// Opens a file for reading and writing. You might want to use base/file.h
// instead.
PlatformFile OpenPlatformFile(const std::string& path, int shared_mode = 0);

// Opens a file for reading only. You might want to use base/file.h
// instead.
PlatformFile OpenPlatformFileReadOnly(const std::string& path);

// Creates a new file for reading and writing. If the file already exists it
// will be overwritten. You might want to use base/file.h instead.
PlatformFile CreatePlatformFile(const std::string& path, int shared_mode = 0);
