#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

class FileSystem {
 public:
  virtual ~FileSystem() = default;

  virtual int open(const char* path, int flags) { return ::open(path, flags); }

  virtual int fstat(int fd, struct stat* buf) { return ::fstat(fd, buf); }

  virtual off_t lseek(int fd, off_t offset, int whence) {
    return ::lseek(fd, offset, whence);
  }

  virtual ssize_t pread(int fd, void* buf, size_t count, off_t offset) {
    return ::pread(fd, buf, count, offset);
  }

  virtual ssize_t pwrite(int fd, const void* buf, size_t count, off_t offset) {
    return ::pwrite(fd, buf, count, offset);
  }

  virtual int close(int fd) { return ::close(fd); }
};

#endif
