#pragma once

#include <stddef.h>
#include <stdint.h>
#include <string>

#include "platform_file.h"
// This class wraps the platform specific APIs for simple file interactions.
//
// The various read and write methods are best effort, i.e. if an underlying
// call does not manage to read/write all the data more calls will be performed,
// until an error is detected or all data is read/written.
class File {
 public:
  // Wraps the given PlatformFile. This class is then responsible for closing
  // the file, which will be done in the destructor if Close is never called.
  explicit File(PlatformFile);
  // The default constructor produces a closed file.
  File();
  ~File();

  File(File&& other);
  File& operator=(File&& other);

  // Open and Create give files with both reading and writing enabled.
  static File Open(const std::string& path, int shared_mode = 0);
  // If the file already exists it will be overwritten.
  static File Create(const std::string& path, int shared_mode = 0);

  // Remove a file in the file system.
  static bool Remove(const std::string& path);

  size_t Write(const uint8_t* data, size_t length);
  size_t Read(uint8_t* buffer, size_t length);

  // The current position in the file after a call to these methods is platform
  // dependent (MSVC gives position offset+length, most other
  // compilers/platforms do not alter the position), i.e. do not depend on it,
  // do a Seek before any subsequent Read/Write.
  size_t WriteAt(const uint8_t* data, size_t length, size_t offset);
  size_t ReadAt(uint8_t* buffer, size_t length, size_t offset);

  // Attempt to position the file at the given offset from the start.
  // Returns true if successful, false otherwise.
  bool Seek(size_t offset);

  // Attempt to close the file. Returns true if successful, false otherwise,
  // most notably when the file is already closed.
  bool Close();

  bool IsOpen();

 private:
  PlatformFile file_;
  File(const File&) = delete;
};
