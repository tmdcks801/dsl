#ifndef MOCK_SENSOR_INTERFACE_H
#define MOCK_SENSOR_INTERFACE_H

#include <gmock/gmock.h>
#include <sensor/sensor_interface.h>
#include <mock/file_system.h>

class MockSensorInterface : public SensorInterface {
 public:
  MockSensorInterface(
      std::unique_ptr<MockFileSystem> fs = std::unique_ptr<MockFileSystem>())
      : SensorInterface(kFakeFd, std::move(fs)) {}

  MOCK_METHOD(std::optional<int>, ReadSensor, (), (noexcept, override));

  static constexpr int kFakeFd = 42;

};

#endif
