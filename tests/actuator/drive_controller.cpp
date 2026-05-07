#include <actuator/drive_controller.h>
#include <core/observer.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <mock/actuator/actuator_interface.h>
#include <mock/core/observer.h>
#include <types/event.h>
#include <types/motor_action.h>

#include <memory>
#include <span>
#include <vector>

using ::testing::_;
using ::testing::Eq;
using ::testing::Return;

class DriveControllerTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // 1. The test fixture now owns the observer
    mock_observer_owner = std::make_unique<MockObserver>();
    mock_observer = mock_observer_owner.get();

    auto mock_left_ptr = std::make_unique<MockActuatorInterface>();
    mock_left_motor = mock_left_ptr.get();

    auto mock_right_ptr = std::make_unique<MockActuatorInterface>();
    mock_right_motor = mock_right_ptr.get();

    auto interfaces = std::make_unique<InterfaceEntry<ActuatorInterface>[]>(2);
    interfaces[0].hardware_interface = std::move(mock_left_ptr);
    interfaces[0].event = Event::kLMotorFault;

    interfaces[1].hardware_interface = std::move(mock_right_ptr);
    interfaces[1].event = Event::kRMotorFault;

    // 2. Pass the RAW pointer (mock_observer) instead of moving a unique_ptr
    controller = std::make_unique<DriveController>(
        mock_observer, std::move(interfaces), "mock_drive_dev");
  }

  // Member variables to handle ownership and access
  std::unique_ptr<MockObserver> mock_observer_owner;  // Keeps the object alive
  MockObserver* mock_observer;                        // Used for EXPECT_CALLs

  MockActuatorInterface* mock_left_motor;
  MockActuatorInterface* mock_right_motor;

  std::unique_ptr<DriveController> controller;
};

// 정상 전진
TEST_F(DriveControllerTest, HappyPathDriving) {
  ActuatorAction expected_action = MotorAction::kForward;

  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));

  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(DriveOperation::kForward);
}
// 정상 좌측 선회
TEST_F(DriveControllerTest, HappyPathLeft) {
  ActuatorAction expected_left = MotorAction::kBackward;
  ActuatorAction expected_right = MotorAction::kForward;

  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_left)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_right)))
      .WillOnce(Return(ActuatorStatus::kFine));

  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(DriveOperation::kLeft);
}

// 정상 정지
TEST_F(DriveControllerTest, HappyPathStop) {
  ActuatorAction expected_action = MotorAction::kStop;

  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kFine));

  EXPECT_CALL(*mock_observer, Notify(_)).Times(0);

  controller->SetOperation(DriveOperation::kStop);
}
// 우측 모터 오작동, 좌측 정상 작동
TEST_F(DriveControllerTest, SadPathRight) {
  ActuatorAction expected_left = MotorAction::kForward;
  ActuatorAction expected_right = MotorAction::kBackward;

  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_left)))
      .WillOnce(Return(ActuatorStatus::kFine));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_right)))
      .WillOnce(Return(ActuatorStatus::kBad));

  EXPECT_CALL(*mock_observer, Notify(Eq(Event::kRMotorFault))).Times(1);

  controller->SetOperation(DriveOperation::kRight);
}

// 양측 모터 오작동
TEST_F(DriveControllerTest, SadPathBoth) {
  ActuatorAction expected_action = MotorAction::kForward;

  EXPECT_CALL(*mock_left_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kBad));
  EXPECT_CALL(*mock_right_motor, SetAction(Eq(expected_action)))
      .WillOnce(Return(ActuatorStatus::kBad));

  EXPECT_CALL(*mock_observer, Notify(Eq(Event::kLMotorFault))).Times(1);
  EXPECT_CALL(*mock_observer, Notify(Eq(Event::kRMotorFault))).Times(1);

  controller->SetOperation(DriveOperation::kForward);
}
