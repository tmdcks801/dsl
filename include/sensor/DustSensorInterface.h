#ifndef DUST_SENSOR_INTERFACE_H
#define DUST_SENSOR_INTERFACE_H

#include <sensor/SensorInterface.h>

class DustSensorInterface : public SensorInterface {
    public:
        using SensorInterface::SensorInterface;

        int readSensor();

};
#endif