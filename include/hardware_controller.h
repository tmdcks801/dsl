#ifndef HARDWARD_CONTROLLER_H
#define HARDWARD_CONTROLLER_H

#include <core/observer.h>
#include <hardware_interface.h>
#include <types/event.h>

#include <concepts>
#include <format>
#include <memory>
#include <span>
#include <string>

template <typename T>
  requires std::derived_from<T, HardwareInterface>
struct InterfaceEntry {
  std::unique_ptr<T> hardware_interface;
  Event event;
};

template <typename T>
  requires std::derived_from<T, HardwareInterface>
class HardwareController {
 public:
  explicit HardwareController(Observer* observer,
                              std::unique_ptr<InterfaceEntry<T>[]> interfaces,
                              std::size_t count,
                              const std::string& file_path_prefix,
                              std::span<const Event> interface_events)
      : observer_(observer),
        interfaces_(std::move(interfaces)),
        interface_count_(count) {
    for (std::size_t i = 0; i < interface_count_; ++i) {
      if (interfaces_[i].hardware_interface != nullptr) {
        continue;
      }
      std::string file_path = std::format("{}{}", file_path_prefix, i);

      auto& [hardware_interface, event] = interfaces_[i];
      hardware_interface = HardwareInterface::Create<T>(file_path);
      event = interface_events[i];
    }
  }

  virtual ~HardwareController() = default;

  virtual void AddObserver(Observer* observer) { observer_ = observer; }

 protected:
  Observer* observer_;
  std::unique_ptr<InterfaceEntry<T>[]> interfaces_;

  const std::size_t interface_count_;
};

#endif
