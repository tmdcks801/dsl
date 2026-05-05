#ifndef RVC_SYSTEM_H
#define RVC_SYSTEM_H

#include <actuator/cleaner_controller.h>
#include <actuator/drive_controller.h>
#include <core/sensor_observer.h>
#include <sensor/dust_sensor_controller.h>
#include <sensor/obstacle_sensor_controller.h>
#include <sensor/sensor_interface.h>
#include <state/rvc_state.h>

#include <array>
#include <chrono>
#include <memory>

using namespace std::chrono_literals;
using Seconds = std::chrono::duration<double>;

class RvcSystem : public SensorObserver {
 public:
  RvcSystem(
      std::unique_ptr<DustSensorController> dust_sensor_controller,
      std::unique_ptr<ObstacleSensorController> obstacle_sensor_controller,
      int frequency, std ::unique_ptr<CleanerController> cleanner_controller,
      std::unique_ptr<DriveController> drive_controller);

  virtual void Notify(Event event) override;
  virtual void Tick();

 private:
  static constexpr std::size_t kSensorControllerCount = 2;
  std::unique_ptr<RvcState> current_state_;
  std::array<std::unique_ptr<SensorController>, kControllerCount>
      sensor_controllers_;

  Seconds tick_interval_;

  std::unique_ptr<RvcState> current_state_;
};
#endif
