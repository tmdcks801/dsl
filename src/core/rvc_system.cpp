#include <core/rvc_system.h>

RvcSystem::RvcSystem(std::unique_ptr<SensorController> dust,
                     std::unique_ptr<SensorController> obstacle) {
  sensor_controllers_[0] = std::move(dust);
  sensor_controllers_[1] = std::move(obstacle);
}

void RvcSystem::Tick() {
      // TODO
}
