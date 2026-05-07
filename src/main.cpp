#include <core/rvc_system.h>

#include <chrono>
#include <stdexcept>
#include <thread>

int main(int argc, char *argv[]) {
  static constexpr int kFrequency = 40;

  auto dust_controller = std::make_unique<DustSensorController>(
      nullptr, std::make_unique<InterfaceEntry<SensorInterface>[]>(1), "DUST");

  auto obstacle_controller = std::make_unique<ObstacleSensorController>(
      nullptr, std::make_unique<InterfaceEntry<SensorInterface>[]>(3),
      "OBSTACLE");

  Seconds tick_interval = 1.0s / kFrequency;

  auto drive_controller = std::make_shared<DriveController>(
      nullptr, std::make_unique<InterfaceEntry<ActuatorInterface>[]>(2),
      "MOTOR");

  auto cleaner_controller = std::make_shared<CleanerController>(
      nullptr, std::make_unique<InterfaceEntry<ActuatorInterface>[]>(1),
      "CLEANER");

  RvcSystem rvc_system(std::move(dust_controller),
                       std::move(obstacle_controller), drive_controller,
                       cleaner_controller, tick_interval);

  auto next_tick = std::chrono::steady_clock::now();

  try {
    while (true) {
        rvc_system.Tick();

        next_tick +=
            std::chrono::duration_cast<std::chrono::steady_clock::duration>(
                tick_interval);

        std::this_thread::sleep_until(next_tick);
      }
  } catch (std::runtime_error error) {
    return -1;
  }
  

  return 0;
}
