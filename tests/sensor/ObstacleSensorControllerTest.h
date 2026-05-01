#include <gtest/gtest.h>
#include <mock/sensor/MockObstacleSensorInterface.h>
#include <sensor/ObstacleSensorController.h>
#include <limits>

using ::testing::Return;

class ObstacleSensorControllerTest : public ::testing::TestWithParam {
    protected:
        MockObstacleSensorInterface mockSensor;
        const int Obstacle_THRESHOLD = 50;
}

INSTANTIATE_TEST_SUITE_P(obstacleDatas, ObstacleThreshold, ::testing::Range(std::numeric_limits<int>::min, std::numeric_limits<int>::max));

// 1. checkSensor()
// 장애물 수치가 임계 값보다 낮을 때
TEST_P(ObstacleThreshold, obstacle_data_low) {
    int obstacleData = getParam();


}

// 장애물 수치가 임계 값과 같을 때
// 장애물 수치가 임계 값보다 클 때
// 장애물 수치가 INT_MAX
// 장애물 수치가 INT_MIN
// 하드웨어가 동작하지 않는 경우는 어떻게 테스트?


// mockSensor 가 null 인 경우

TEST_F(ObstacleSensorInterfaceTest, /* 테스트 이름*/) {
    EXPECT_CALL(/* 누가 */, /* 무엇을 호출 (메서드)*/)
    .WillOnce(Return(/* 리턴 값 */)) /* 한번만 호출*/;

    // ....
}