#ifndef HARDWARD_CONTROLLER_H
#define HARDWARD_CONTROLLER_H

#include <core/observer.h>
#include <types/event.h>
#include <hardware_interface.h>

#include <memory>
#include <string>
#include <span>
#include <concepts>

enum class Event;

template<typename T>
requires std::derived_from<T, HardwareInterface>
struct InterfaceEntry {
  std::unique_ptr<HardwareInterface> hardware_interface;
  Event event;
};


template <typename T>
  requires std::derived_from<T, HardwareInterface>
class HardwareController {
 public:
  explicit HardwareController(std::unique_ptr<Observer> observer,
                              std::unique_ptr<InterfaceEntry<T>[]> interfaces,
                              std::size_t count,
                              const std::string &file_path_prefix,
                              std::span<const Event> interface_events);

  virtual ~HardwareController() = default;

 protected:
  static constexpr Event kHWError = Event::kHWFault;

  std::unique_ptr<Observer> observer_;
  std::unique_ptr<InterfaceEntry<T>[]> interfaces_;
  
  const std::size_t interface_count_;
};

#endif
