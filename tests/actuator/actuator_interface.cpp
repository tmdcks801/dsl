#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <actuator/actuator_interface.h>
#include <file_system.h>
#include <types/actuator_status.h>
#include <hardware_interface.h>
#include <types/motor_action.h>
#include <types/cleaner_action.h>

#include <mock/file_system.h>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::NiceMock;


class ActuatorInterfaceTest : public ::testing::Test {
 protected:
  void SetUp() override {
    auto mock_fs = std::make_unique<NiceMock<MockFileSystem>>();
    mock_fs_ptr_ = mock_fs.get(); 

    ON_CALL(*mock_fs_ptr_, open(_, _)).WillByDefault(Return(fake_fd_));

    actuator_ = HardwareInterface::Create<ActuatorInterface>(
        "/dev/dummy_actuator", 
        std::move(mock_fs)
    );
    ASSERT_NE(actuator_, nullptr);
  }

  static constexpr int kActuatorStatusPos = 0;
  static constexpr int kActuatorActionPos = sizeof(int);

  int fake_fd_ = 42;
  MockFileSystem* mock_fs_ptr_; 
  std::unique_ptr<ActuatorInterface> actuator_;
};

//GetStatus() 테스트
//파일 읽기 실패(int 사이즈가 아닌 0바이트 읽기)
TEST_F(ActuatorInterfaceTest, PreadFail) {
  EXPECT_CALL(*mock_fs_ptr_, pread(fake_fd_, _, sizeof(int), kActuatorStatusPos)) 
      .WillOnce(Return(0)); 

  EXPECT_EQ(actuator_->GetStatus(), ActuatorStatus::kBad);
}
//파일 읽기 성공, 파일 내용물 오류(ActuatorStatus가 bad함)
TEST_F(ActuatorInterfaceTest, RawStatusNotOne) {
  EXPECT_CALL(*mock_fs_ptr_, pread(fake_fd_, _, sizeof(int), kActuatorStatusPos))
      .WillOnce([](int, void* buf, size_t, off_t) {
        *static_cast<int*>(buf) = 0;
        return sizeof(int);
      });

  EXPECT_EQ(actuator_->GetStatus(), ActuatorStatus::kBad);
}
//파일 읽기 성공, 파일 내용물 정상
TEST_F(ActuatorInterfaceTest, HappyPath) {
  EXPECT_CALL(*mock_fs_ptr_, pread(fake_fd_, _, sizeof(int), kActuatorStatusPos))
      .WillOnce([](int, void* buf, size_t, off_t) {
        *static_cast<int*>(buf) = 1;
        return sizeof(int);
      });

  EXPECT_EQ(actuator_->GetStatus(), ActuatorStatus::kFine);
}


//SetAction 테스트
//pwrite 실패(int 사이즈 아닌 0 return)
TEST_F(ActuatorInterfaceTest, BadStatusAndReturnBad) {
  ActuatorAction action = CleanerAction::kOn;

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorActionPos)) 
      .WillOnce(Return(0));

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorStatusPos)) 
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), 0);
        return sizeof(int);
      });

  EXPECT_EQ(actuator_->SetAction(action), ActuatorStatus::kBad);
}
//모터 정상 작동
TEST_F(ActuatorInterfaceTest, HappyMotor) {
  ActuatorAction action = MotorAction::kBackward;

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorActionPos))
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), static_cast<int>(MotorAction::kBackward));
        return sizeof(int);
      });

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorStatusPos))
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), 1);
        return sizeof(int);
      });

  EXPECT_EQ(actuator_->SetAction(action), ActuatorStatus::kFine);
}

//enum 대신 int 정상 작동
TEST_F(ActuatorInterfaceTest, HappyInt) {
  ActuatorAction action = 99;

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorActionPos))
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), 99);
        return sizeof(int);
      });

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorStatusPos))
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), 1);
        return sizeof(int);
      });

  EXPECT_EQ(actuator_->SetAction(action), ActuatorStatus::kFine);
}

//클리너 정상 작동
TEST_F(ActuatorInterfaceTest, HappyCleaner) {
  ActuatorAction action = CleanerAction::kPowerUp; 

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorActionPos))
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), static_cast<int>(CleanerAction::kPowerUp));
        return sizeof(int);
      });

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorStatusPos))
      .WillOnce([](int, const void* buf, size_t, off_t) {
        EXPECT_EQ(*static_cast<const int*>(buf), 1);
        return sizeof(int);
      });

  EXPECT_EQ(actuator_->SetAction(action), ActuatorStatus::kFine);
}

//pwrite 1단계 성공, 2단계 실패
TEST_F(ActuatorInterfaceTest, 1stFails_2ndFine) {
  ActuatorAction action = CleanerAction::kOn;

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorActionPos))
      .WillOnce(Return(sizeof(int)));

  EXPECT_CALL(*mock_fs_ptr_, pwrite(fake_fd_, _, sizeof(int), kActuatorStatusPos))
      .WillOnce(Return(0));

  EXPECT_EQ(actuator_->SetAction(action), ActuatorStatus::kFine);
}
