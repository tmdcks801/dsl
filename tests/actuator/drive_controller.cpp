#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <actuator/drive_controller.h>
#include <core/observer.h>
#include <types/event.h>
#include <types/motor_action.h>

#include <mock/actuator/actuator_interface.h>
#include <mock/core/observer.h>

#include <memory>
#include <vector>

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;


class DriveControllerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mock_observer_ptr = std::make_unique<MockObserver>();
    mock_observer = mock_observer_ptr.get();

    auto mock_left_ptr = std::make_unique<MockActuatorInterface>();
    mock_left_motor = mock_left_ptr.get();

    auto mock_right_ptr = std::make_unique<MockActuatorInterface>();
    mock_right_motor = mock_right_ptr.get();

    // The DriveController requires 2 actuator interfaces (0 = left, 1 = right)
    auto interfaces = std::make_unique<InterfaceEntry<ActuatorInterface>[]>(2);
    interfaces[0].hardware_interface = std::move(mock_left_ptr);
    interfaces[0].event = Event::kHWFault;
    interfaces[1].hardware_interface = std::move(mock_right_ptr);
    interfaces[1].event = Event::kHWFault;

    std::vector<Event> events = {Event::kHWFault, Event::kHWFault};

    // Instantiate the controller with our injected mocks
    controller = std::make_unique<DriveController>(
        std::move(mock_observer_ptr), 
        std::move(interfaces), 
        2, 
        "mock_drive_dev", 
        events);
  }

  // Raw pointers retained for setting EXPECT_CALLs
  MockObserver* mock_observer;
  MockActuatorInterface* mock_left_motor;
  MockActuatorInterface* mock_right_motor;
  
  // The unit under test
  std::unique_ptr<DriveController> controller;
};

// =====================================================================
// Test Cases
// =====================================================================

TEST_F(DriveControllerTest, SetOperation_DriveForwardSuccessfully) {
  ActuatorAction expected_action = MotorAction::kForward;

  // Expect both motors to receive the kForward action
  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));

  // Observer should NOT be notified
  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(DriveOperation::kForward);
}

TEST_F(DriveControllerTest, SetOperation_DriveLeftSuccessfully) {
  ActuatorAction expected_left = MotorAction::kBackward;
  ActuatorAction expected_right = MotorAction::kForward;

  // Left turn means left wheel backward, right wheel forward
  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_left)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_right)))
      .WillOnce(Return(ActuatorStatus::kFine));

  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(DriveOperation::kLeft);
}

TEST_F(DriveControllerTest, SetOperation_StopSuccessfully) {
  ActuatorAction expected_action = MotorAction::kStop;

  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));

  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(DriveOperation::kStop);
}

TEST_F(DriveControllerTest, SetOperation_RightMotorFaultTriggersNotification) {
  ActuatorAction expected_left = MotorAction::kForward;
  ActuatorAction expected_right = MotorAction::kBackward; // For a right turn

  // Left motor succeeds, but right motor fails
  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_left)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_right)))
      .WillOnce(Return(ActuatorStatus::kBad));

  // The observer MUST be notified exactly once with a kHWFault event
  EXPECT_CALL(*mock_observer, Notify(Eq(Event::kHWFault))).Times(1);

  controller->SetOperation(DriveOperation::kRight);
}

TEST_F(DriveControllerTest, SetOperation_UnknownCommandTriggersFault) {
  // If an unmapped enum value is passed, neither motor should be called
  EXPECT_CALL(*mock_left_motor, SetAction(_)).Times(0);
  EXPECT_CALL(*mock_right_motor, SetAction(_)).Times(0);

  // The default switch case should catch it and trigger a fault
  EXPECT_CALL(*mock_observer, Notify(Eq(Event::kHWFault))).Times(1);

  // Cast an invalid int to DriveOperation to trigger the default switch case
  controller->SetOperation(static_cast<DriveOperation>(999));
}