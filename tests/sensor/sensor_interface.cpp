#include <mock/file_system.h>
#include <sensor/sensor_interface.h>

#include <cstring>
#include <memory>
#include <optional>

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::Eq;
using ::testing::Optional;

class SensorInterfaceTest : public ::testing::Test {
 protected:
  static constexpr std::string kTestFileName = "OBSTACLE_t";
  static constexpr int kFakeFd = 42;

  std::unique_ptr<MockFileSystem> mock_fs_;
  MockFileSystem *mock_ptr_;

  void SetUp() override {
    mock_fs_ = std::make_unique<MockFileSystem>();
    mock_ptr_ = mock_fs_.get();
  }

  void TearDown() override {}
};

// ReadSensor 성공 (정확히 int 크기 만큼 읽기)
TEST_F(SensorInterfaceTest, ReadSensorSuccess) {
  EXPECT_CALL(*mock_ptr_, open(_, _)).WillOnce(Return(kFakeFd));

  EXPECT_CALL(*mock_ptr_, fstat(kFakeFd, _))
      .WillOnce([](int fd, struct stat *buf) {
        buf->st_size = sizeof(int);
        return 0;
      });

  constexpr int expected_value = 0x12345678;

  EXPECT_CALL(*mock_ptr_, pread(kFakeFd, _, sizeof(int), _))
      .WillOnce(DoAll(
          testing::Invoke([expected_value](int fd, void *buf, size_t count, off_t offset) {
            std::memcpy(buf, &expected_value, sizeof(int));
          }),

          Return(sizeof(int))));

  EXPECT_CALL(*mock_ptr_, close(kFakeFd)).WillOnce(Return(0));

  auto sensor = HardwareInterface::Create<SensorInterface>(kTestFileName,
                                                           std::move(mock_fs_));
  auto result = sensor->ReadSensor();

  ASSERT_TRUE(result.has_value());
  EXPECT_THAT(result, Optional(expected_value));
}

// ReadSensor 실패 (파일 크기가 int 미만)
TEST_F(SensorInterfaceTest, ReadSensorFailWhenPartialSize) {
  EXPECT_CALL(*mock_ptr_, open(_, _)).WillOnce(Return(kFakeFd));

  //+ 더 작은 크기의 파일 mock 추가
  EXPECT_CALL(*mock_ptr_, fstat(kFakeFd, _))
      .WillOnce([](int fd, struct stat *buf) {
        buf->st_size = sizeof(int) - 1; 
        return 0;
      });

  EXPECT_CALL(*mock_ptr_, pread(kFakeFd, _, sizeof(int), 0)).Times(0);

  EXPECT_CALL(*mock_ptr_, close(kFakeFd)).WillOnce(Return(0));

  auto sensor = HardwareInterface::Create<SensorInterface>(kTestFileName,
                                                           std::move(mock_fs_));
  auto result = sensor->ReadSensor();

  EXPECT_THAT(result, Eq(std::nullopt));
}

// ReadSensor 실패 (파일 크기가 int 같음 && 실제 읽은 데이터는 적음)
TEST_F(SensorInterfaceTest, ReadSensorFailWhenPartialRead) {
  EXPECT_CALL(*mock_ptr_, open(_, _)).WillOnce(Return(kFakeFd));

  EXPECT_CALL(*mock_ptr_, fstat(kFakeFd, _))
      .WillOnce([](int fd, struct stat *buf) {
        buf->st_size = 4;
        return 0;
      });

  EXPECT_CALL(*mock_ptr_, pread(_, _, _, _)).WillOnce(Return(2));

    EXPECT_CALL(*mock_ptr_, close(kFakeFd)).WillOnce(Return(0));

   auto sensor = HardwareInterface::Create<SensorInterface>(
        kTestFileName, std::move(mock_fs_));

   auto result =  sensor->ReadSensor();

   EXPECT_THAT(result, Eq(std::nullopt));
}

// ReadSensor 실패 (파일 크기가 int 초과)
TEST_F(SensorInterfaceTest, ReadSensorFailWhenOverSize) {
  EXPECT_CALL(*mock_ptr_, open(_, _)).WillOnce(Return(kFakeFd));

  EXPECT_CALL(*mock_ptr_, fstat(kFakeFd, _))
      .WillOnce([](int fd, struct stat *buf) {
        buf->st_size = 8;
        return 0;
      });

  EXPECT_CALL(*mock_ptr_, pread(_, _, _, _)).Times(0);

  EXPECT_CALL(*mock_ptr_, close(kFakeFd)).WillOnce(Return(0));

  auto sensor = HardwareInterface::Create<SensorInterface>(kTestFileName,
                                                           std::move(mock_fs_));
  auto result = sensor->ReadSensor();

  EXPECT_THAT(result, Eq(std::nullopt));
}

//+ ReadSensor 실패 (fstat 자체 에러) 추가
TEST_F(SensorInterfaceTest, ReadSensorFailWhenFstatFails) {
  EXPECT_CALL(*mock_ptr_, open(_, _)).WillOnce(Return(kFakeFd));

  EXPECT_CALL(*mock_ptr_, fstat(kFakeFd, _)).WillOnce(Return(-1));

  EXPECT_CALL(*mock_ptr_, pread(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_ptr_, close(kFakeFd)).WillOnce(Return(0));

  auto sensor = HardwareInterface::Create<SensorInterface>(kTestFileName,
                                                           std::move(mock_fs_));
  auto result = sensor->ReadSensor();

  EXPECT_THAT(result, Eq(std::nullopt));
}
