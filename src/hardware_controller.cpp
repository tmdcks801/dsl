#include <hardware_controller.h>
#include <hardware_interface.h>

#include <format>

template <typename T>
requires std::derived_from<T, HardwareInterface>
HardwareController<T>::HardwareController(
    std::unique_ptr<Observer> observer,
    std::unique_ptr<InterfaceEntry<T>[]> interfaces, std::size_t count,
    const std::string& file_path_prefix,
    std::span<const Event> interface_events)
    : observer_(std::move(observer)),
      interfaces_(std::move(interfaces)),
      interface_count_(count) {
  for (int i = 0; i < interface_count_; ++i) {
    std::string file_path = std::format("{}{}", file_path_prefix, i);

    auto& [hardware_interface, event] = interfaces_[i];
    hardware_interface = HardwareInterface::Create<T>(file_path);
    event = interface_events[i];

    if (hardware_interface == nullptr) {
      observer_->Notify(kHWError);
      break;
    }
  }
}
