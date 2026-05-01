#include <gtest/gtest.h>
#include <mock/sensor/MockDustSensorInterface.h>
#include <mock/system/MockSensorObserver.h>
#include <sensor/DustSensorController.h>
#include <limits>

using ::testing::Return;

class DustSensorControllerTest : public ::testing::TestWithParam<int> {
    protected:
        MockDustSensorInterface mockSensor;
        const int DUST_THRESHOLD = 50;

    void SetUp() override {
        
    }

    void TearDown() override {

    }
}

INSTANTIATE_TEST_SUITE_P(dustDatas, DustThreshold, ::testing::Range(std::numeric_limits<int>::min, std::numeric_limits<int>::max));
// dust raw data : [INT_MIN, INT_MAX]

// 1. checkSensor()
// 먼지 농도가 임계 값보다 낮을 때
TEST_P(DustThreshold, dust_data_low) {
    int dustData = GetParam();
    DustSensorController controller(&mockSensor, DUST_THRESHOLD);

    EXPECT_CALL(mockSensor, readSensor())
        .WillOnce(Return(dustData));

    EXPECT_CALL(mockObserver, notify(Event::DustObstacle)).Times(0);

    controller.checkSensor();
}

// 먼지 농도가 임계 값과 같을 때
// 먼지 농도가 임계 값보다 클 때
// 먼지 농도가 INT_MAX
// 먼지 농도가 INT_MIN
// 하드웨어가 동작하지 않는 경우는 어떻게 테스트?


// mockSensor 가 null 인 경우

TEST_F(DustSensorControllerTest, /* 테스트 이름*/) {
    EXPECT_CALL(/* 누가 */, /* 무엇을 호출 (메서드)*/)
    .WillOnce(Return(/* 리턴 값 */)) /* 한번만 호출*/;

    // ....
}