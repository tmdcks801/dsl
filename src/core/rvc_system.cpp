#include <core/rvc_system.h>

RvcSystem::RvcSystem(
    std::unique_ptr<DustSensorController> dust_sensor_controller,
    std::unique_ptr<ObstacleSensorController> obstacle_sensor_controller,
    int frequency, std::unique_ptr<CleanerController> cleanner_controller,
    std::unique_ptr<DriveController> drive_controller,
    std::unique_ptr<RvcState> state) {
  sensor_controllers_[0] = std::move(dust_sensor_controller);
  sensor_controllers_[1] = std::move(obstacle_sensor_controller);

  tick_interval_ = Seconds(1.0 / frequency);

  current_state_ = std::move(state);
}

RvcSystem::RvcSystem(
    std::unique_ptr<DustSensorController> dust_sensor_controller,
    std::unique_ptr<ObstacleSensorController> obstacle_sensor_controller,
    int frequency, std::unique_ptr<CleanerController> cleanner_controller,
    std::unique_ptr<DriveController> drive_controller) {}

void RvcSystem::Notify(Event event) {
  std::unique_ptr<RvcState> next_state = current_state_->Handle(event);

  if (next_state != nullptr) {
    current_state_->Exit();
    current_state_ = std::move(next_state);
    current_state_->Enter();
  }
}

void RvcSystem::Tick() {
  for (int i = 0; i < kSensorControllerCount; ++i) {
    sensor_controllers_[i]->CheckSensor();
  }

  current_state_->tick(tick_interval_);
}
