#ifndef CLEANER_CONTROLLER_H
#define CLEANER_CONTROLLER_H
#include <actuator/actuator_controller.h>

#include <array>

class CleanerController : public ActuatorController {
 public:
  explicit CleanerController(
      Observer* observer,
      std::unique_ptr<InterfaceEntry<ActuatorInterface>[]> interfaces,
      const std::string& file_path_prefix)
      : ActuatorController(observer, std::move(interfaces), kMaxActuator,
                           file_path_prefix, actuator_events_) {}

  void SetOperation(ActuatorOperation operation) override;

 private:
  static constexpr std::size_t kMaxActuator = 1;

  static constexpr std::array<Event, kMaxActuator> actuator_events_ = {
      Event::kCleanerFault
  };
};

#endif
