#ifndef OBSTACLE_SENSOR_CONTROLLER_H
#define OBSTACLE_SENSOR_CONTROLLER_H

#include <sensor/SensorController.h>
#include <sensor/ObstacleSensorInterface.h>
#include <array>
#include <map>

enum class Event;

using std::map<int, std::pair<ObstacleSensorInterface*, Event>> = SensorMap;

class ObstacleSensorController : public SensorController{
private :
        const int OBSTALCE_THRESHOLD;
        SensorMap map;
        
public :

        ObstacleSensorController(SensorMap map){
            this.map=map;
        }
        
        void checkSensor() override {
            /////////아앍앙람ㅇㄴ마
        }
};
#endif