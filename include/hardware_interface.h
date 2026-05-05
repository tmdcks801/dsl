#ifndef HARDWARE_INTERFACE_H
#define HARDWARE_INTERFACE_H

#include <file_system.h>

#include <concepts>
#include <memory>
#include <string>

class HardwareInterface {
 public:
  HardwareInterface(const HardwareInterface &) = delete;
  HardwareInterface &operator=(const HardwareInterface &) = delete;

  HardwareInterface(HardwareInterface &&) noexcept = default;
  HardwareInterface &operator=(HardwareInterface &&) noexcept = default;

  virtual ~HardwareInterface();

  template <typename T>
    requires std::derived_from<T, HardwareInterface>
  static std::unique_ptr<T> Create(
      const std::string &file_name,
                                   std::unique_ptr<FileSystem> file_system =
                                       std::make_unique<FileSystem>()) {
    const int file_descriptor =
        file_system->open(file_name.c_str(), O_RDONLY | O_NONBLOCK);

    if (file_descriptor == -1) {
      return nullptr;
    }

    return std::unique_ptr<T>(new T(file_descriptor, std::move(file_system)));
  }

 protected:
  explicit HardwareInterface(const int device_id,
                             std::unique_ptr<FileSystem> file_system) noexcept
      : device_id_(device_id), file_system_(std::move(file_system)) {};

  const int device_id_;
  std::unique_ptr<FileSystem> file_system_;
};

#endif
