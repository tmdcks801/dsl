#include <core/rvc_system.h>
#include <state/forward_state.h>
#include <stdexcept>

RvcSystem::RvcSystem(
    std::unique_ptr<DustSensorController> dust_sensor_controller,
    std::unique_ptr<ObstacleSensorController> obstacle_sensor_controller,
    std::shared_ptr<DriveController> drive_controller,
    std::shared_ptr<CleanerController> cleaner_controller,
    Seconds tick_interval,
    std::unique_ptr<RvcState> state)
    : tick_interval_(tick_interval) {
  sensor_controllers_[0] = std::move(dust_sensor_controller);
  sensor_controllers_[0]->AddObserver(this);

  sensor_controllers_[1] = std::move(obstacle_sensor_controller);
  sensor_controllers_[1]->AddObserver(this);

  if (state == nullptr) {
    current_state_ = std::make_unique<ForwardState>(
        this, drive_controller, cleaner_controller);
  } else {
    current_state_ = std::move(state);
  }

  current_state_->Enter();
  
}

void RvcSystem::Notify(Event event) {
  if (event == Event::kSystemShutdown) {
    throw std::runtime_error("EMERGENCY STOP");
  }
  
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

  current_state_->Tick(tick_interval_);
}
