#ifndef MOCK_ACTUATOR_INTERFACE_H
#define MOCK_ACTUATOR_INTERFACE_H

#include <gmock/gmock.h>
#include <actuator/actuator_interface.h>

class MockActuatorInterface : public ActuatorInterface {
 public:
  // Injecting -1 and nullptr to bypass the real HardwareInterface FileSystem logic
  MockActuatorInterface() : ActuatorInterface(-1, nullptr) {}
  
  MOCK_METHOD(ActuatorStatus, GetStatus, (), (override));
  MOCK_METHOD(ActuatorStatus, SetAction, (ActuatorAction action), (override));
};

#endif
