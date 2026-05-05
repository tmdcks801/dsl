#ifndef RVC_SYSTEM_H
#define RVC_SYSTEM_H

#include <core/sensor_observer.h>
#include <sensor/sensor_controller.h>
#include <state/rvc_state.h>

#include <array>
#include <memory>

class RvcSystem : public SensorObserver {
 public:
  RvcSystem(std::unique_ptr<SensorController> dust,
            std::unique_ptr<SensorController> obstacle);

  virtual void Tick();

 private:
  static constexpr std::size_t kControllerCount = 2;
  std::unique_ptr<RvcState> current_state_;
  std::array<std::unique_ptr<SensorController>, kControllerCount>
      sensor_controllers_;

  // TODO: Actuator 추가
};
#endif
