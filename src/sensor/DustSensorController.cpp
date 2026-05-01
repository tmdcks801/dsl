#include <sensor/DustSensorController.h>

#include <iostream>


enum class Event;

void DustSensorController::checkSensor(){
    for(int i=0; i<sensors.size(); i++) {
        if (sensors[i] != nullptr){
            int dust = sensors[i]->readSensor();
            if(dust==-1){
                //여기서 에러 던지기
            }
            if(dust > DUST_THRESHOLD){
                observer->notify(Event::HighDust);
            }
        }
    }
}