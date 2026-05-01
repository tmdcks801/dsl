#include <gtest/gtest.h>
#include <sensor/DustSensorInterface.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/stat.h>

class DustSensorInterfaceTest : public ::testing::Test {
    protected:
        void SetUp() override {
            std::remove("dust.txt");
        }

        void TearDown() override {
            chmod("dust.txt", 0644);
            std::remove("dust.txt");
        }

        void CreateDustFile(const std::string& content) {
            std::ofstream outfile("dust.txt");
            outfile << content;
            outfile.close();
        }
};

// 정상 테스트
TEST_F(DustSensorInterfaceTest, ReadValidData) {
    CreateDustFile("75");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), 75);
}
TEST_F(DustSensorInterfaceTest, ReadValidData2) {
    CreateDustFile("   75  ");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), 75);
}
TEST_F(DustSensorInterfaceTest, ReadValidData3) {
    CreateDustFile("-3");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -3);
}

TEST_F(DustSensorInterfaceTest, ReadValidData4) {
    CreateDustFile("1.5");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), 1);
}

TEST_F(DustSensorInterfaceTest, ReadValidData5) {
    CreateDustFile("-2147483648");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -2147483648);
}

TEST_F(DustSensorInterfaceTest, ReadValidData6) {
    CreateDustFile("2147483647");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), 2147483647);
}


// 숫자가 아닌 데이터 테스트
TEST_F(DustSensorInterfaceTest, ReadInvalidData1) {
    CreateDustFile("제육볶음");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

TEST_F(DustSensorInterfaceTest, ReadInvalidData2) {
    CreateDustFile("김김승승찬찬입니다");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

//엄청 큰 수 테스트
TEST_F(DustSensorInterfaceTest, ReadInvalidData3) {
    CreateDustFile("99999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

TEST_F(DustSensorInterfaceTest, ReadInvalidData4) {
    CreateDustFile("-99999999999999999999999999999999999999999999999999999999999999999999999999999999999");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

TEST_F(DustSensorInterfaceTest, ReadInValidData5) {
    CreateDustFile("-2147483649");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

TEST_F(DustSensorInterfaceTest, ReadInValidData6) {
    CreateDustFile("2147483648");
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}


// 파일이 없을 때 테스트
TEST_F(DustSensorInterfaceTest, FileNotFound) {
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

// 빈 파일 테스트
TEST_F(DustSensorInterfaceTest, EmptyFile) {
    CreateDustFile(""); // WriteToFile -> CreateDustFile로 수정
    DustSensorInterface sensor(1, Event());
    EXPECT_EQ(sensor.readSensor(), -1);
}

TEST_F(DustSensorInterfaceTest, PermissionDenied) {
    CreateDustFile("100");
    
    chmod("dust.txt", 0000);
    
    DustSensorInterface sensor(3, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}