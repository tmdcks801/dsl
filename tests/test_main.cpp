//#include <gmock/gmock.h>
//#include <gtest/gtest.h>
//
//// Include your headers here...
//#include <sensor/dust_sensor_controller.h>
//#include <sensor/obstacle_sensor_controller.h>
//#include <system/sensor_observer.h>
//
//using ::testing::_;
//using ::testing::Exactly;
//
//class MockSensorObserver : public SensorObserver {
// public:
//  // Assuming you make Notify virtual in SensorObserver
//  MOCK_METHOD(void, Notify, (Event e), (override));
//  MOCK_METHOD(void, SetObserver, (SensorObserver * obs), (override));
//};
//
//class DustSensorControllerTest : public ::testing::Test {
// protected:
//  // We define the threshold here so our tests can use it dynamically
//  const int kThreshold = std::numeric_limits<int>::max() / 2;
//};
//
//TEST_F(DustSensorControllerTest, CheckValueThresholds) {
//  // Setup
//  auto mock_observer = std::make_unique<MockSensorObserver>();
//
//  // Note: Because the base class constructor tries to open "/dev/dummy0",
//  // this test assumes the file either exists, or you handle the nullptr
//  // internally.
//  DustSensorController dust_sensor(std::move(mock_observer), "/dev/dummy");
//
//  // Positive Testing: High dust (greater than threshold) returns true
//  EXPECT_TRUE(dust_sensor.CheckValue(kThreshold + 1));
//  EXPECT_TRUE(dust_sensor.CheckValue(kThreshold + 1000));
//
//  // Boundary Testing: Exactly at threshold returns false
//  EXPECT_FALSE(dust_sensor.CheckValue(kThreshold));
//
//  // Negative Testing: Low dust returns false
//  EXPECT_FALSE(dust_sensor.CheckValue(kThreshold - 1));
//  EXPECT_FALSE(dust_sensor.CheckValue(0));
//}
//
//class ObstacleSensorControllerTest : public ::testing::Test {
// protected:
//  const int kThreshold = std::numeric_limits<int>::max() / 2;
//};
//
//TEST_F(ObstacleSensorControllerTest, CheckValueThresholds) {
//  auto mock_observer = std::make_unique<MockSensorObserver>();
//  ObstacleSensorController obs_sensor(std::move(mock_observer), "/dev/dummy");
//
//  // Positive Testing: Obstacle detected (less than threshold) returns true
//  EXPECT_TRUE(obs_sensor.CheckValue(kThreshold - 1));
//  EXPECT_TRUE(obs_sensor.CheckValue(0));
//
//  // Boundary Testing: Exactly at threshold returns false
//  EXPECT_FALSE(obs_sensor.CheckValue(kThreshold));
//
//  // Negative Testing: No obstacle returns false
//  EXPECT_FALSE(obs_sensor.CheckValue(kThreshold + 1));
//}
