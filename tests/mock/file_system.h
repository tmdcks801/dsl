#include <file_system.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

class MockFileSystem : public FileSystem {
 public:
  MOCK_METHOD(int, open, (const char* path, int flags), (override));
  MOCK_METHOD(off_t, lseek, (int fd, off_t offset, int whence), (override));
  MOCK_METHOD(ssize_t, pread, (int fd, void* buf, size_t count, off_t offset),
              (override));
  MOCK_METHOD(ssize_t, pwrite,
              (int fd, const void* buf, size_t count, off_t offset),
              (override));
  MOCK_METHOD(int, close, (int fd), (override));
  MOCK_METHOD(int, fstat, (int fd, struct stat*), (override));
};
