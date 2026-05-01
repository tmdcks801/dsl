#include <sensor/ObstacleSensorController.h>

enum class Event;

void ObstacleSensorController :: checkSensor(){

    for(int i=0; i<sensors.size(); i++){

        if (sensors[i] != nullptr){

            int obstacle = sensors[i]->readSensor();
            if(obstacle == -1) {
                //에러
            }
            
            if(obstacle > OBSTALCE_THRESHOLD){
                observer->notify(*(map.find(i)));
            }   
        }
    }
}