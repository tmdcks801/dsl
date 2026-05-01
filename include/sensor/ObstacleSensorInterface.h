#ifndef OBSTACLE_SENSOR_INTERFACE_H
#define OBSTACLE_SENSOR_INTERFACE_H

#include <sensor/SensorInterface.h>

class ObstacleSensorInterface : public SensorInterface{
    public:
        using SensorInterface :: SensorInterface;

        int readSensor() override;
};

#endif