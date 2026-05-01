#include <gtest/gtest.h>
#include <sensor/ObstacleSensorInterface.h>
#include <fstream>
#include <string>
#include <cstdio>
#include <sys/stat.h>

class ObstacleSensorInterfaceTest : public ::testing ::Test{
    protected:
        void SetUp() override {
            std::remove("obstacle.txt");
        }

        void TearDown() override {
            chmod("obstacle.txt", 0644);
            std::remove("obstacle.txt");
        }

        void CreateObstacleFile(const std::string& content) {
            std::ofstream outfile("obstacle.txt");
            outfile << content;
            outfile.close();
        }
};

TEST_F(ObstacleSensorInterfaceTest, ReadCorrectColumnValue1) {
    CreateObstacleFile("10 20 30");
    
    ObstacleSensorInterface sensor(0, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, 10);
}

TEST_F(ObstacleSensorInterfaceTest, ReadCorrectColumnValue2) {
    CreateObstacleFile("10 20 30");
    
    ObstacleSensorInterface sensor(1, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, 20);
}

TEST_F(ObstacleSensorInterfaceTest, ReadCorrectColumnValue3) {
    CreateObstacleFile("10 20 30");
    
    ObstacleSensorInterface sensor(2, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, 30);
}

TEST_F(ObstacleSensorInterfaceTest, ReadCorrectColumnValue4) {
    CreateObstacleFile("10 -2147483648 30");
    
    ObstacleSensorInterface sensor(1, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -2147483648);
}

TEST_F(ObstacleSensorInterfaceTest, ReadCorrectColumnValue6) {
    CreateObstacleFile("10 20 2147483647");
    
    ObstacleSensorInterface sensor(2, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, 2147483647);
}
// 스페이스 여러 개와 탭 혼용
TEST_F(ObstacleSensorInterfaceTest, MultipleWhitespaces) {
    CreateObstacleFile("10    20\t30"); 
    ObstacleSensorInterface sensor(1, Event()); 
    EXPECT_EQ(sensor.readSensor(), 20);
}

TEST_F(ObstacleSensorInterfaceTest, FloatingPointData) {
    CreateObstacleFile("75.9 34 12");
    ObstacleSensorInterface sensor(0, Event());
    EXPECT_EQ(sensor.readSensor(), 75); 
}

//내용 빔
TEST_F(ObstacleSensorInterfaceTest, IndexOutOfBounds) {
    CreateObstacleFile("10 20"); 

    ObstacleSensorInterface sensor(2, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}
 // 없는 ID
TEST_F(ObstacleSensorInterfaceTest, InvalidSensorId5) {
    CreateObstacleFile("10 20 30");
    ObstacleSensorInterface sensor(99, Event()); 
    EXPECT_EQ(sensor.readSensor(), -1);
}

//한글아닌거
TEST_F(ObstacleSensorInterfaceTest, InvalidDataTypeInColumn1) {
    CreateObstacleFile("10 제육볶음 30");
    
    ObstacleSensorInterface sensor(1, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

TEST_F(ObstacleSensorInterfaceTest, InvalidDataTypeInColumn2) {
    CreateObstacleFile("김김찬찬승승김김 30 10");
    
    ObstacleSensorInterface sensor(0, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

//짱 큰수
TEST_F(ObstacleSensorInterfaceTest, InvalidDataTypeInColumn3) {
    CreateObstacleFile("10 30 999999999999999999999999999999999999999999999999999999999999999999");
    
    ObstacleSensorInterface sensor(2, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}


TEST_F(ObstacleSensorInterfaceTest, InvalidDataTypeInColumn4) {
    CreateObstacleFile("10 30 -999999999999999999999999999999999999999999999999999999999999999999");
    
    ObstacleSensorInterface sensor(2, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

TEST_F(ObstacleSensorInterfaceTest, InvalidDataTypeInColumn5) {
    CreateObstacleFile("10 -2147483649 30");
    
    ObstacleSensorInterface sensor(1, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

TEST_F(ObstacleSensorInterfaceTest, InvalidDataTypeInColumn6) {
    CreateObstacleFile("10 20 2147483648");
    
    ObstacleSensorInterface sensor(2, Event()); 
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

//권한부족
TEST_F(ObstacleSensorInterfaceTest, PermissionDenied) {
    CreateObstacleFile("10 20 30");
    chmod("obstacle.txt", 0000); 
    
    ObstacleSensorInterface sensor(0, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

//파일 없음
TEST_F(ObstacleSensorInterfaceTest, FileNotFound) {
    ObstacleSensorInterface sensor(0, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}

//내용 빔
TEST_F(ObstacleSensorInterfaceTest, EmptyFile) {
    CreateObstacleFile("");
    
    ObstacleSensorInterface sensor(0, Event());
    int result = sensor.readSensor();
    
    EXPECT_EQ(result, -1);
}