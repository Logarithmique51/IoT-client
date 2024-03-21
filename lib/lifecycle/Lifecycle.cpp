#include "Lifecycle.h"
#include <ESP8266WiFi.h>


std::map<unsigned long, void(*)()> Lifecycle::array_lifecycle;

unsigned long Lifecycle::last_time = 0;

Lifecycle::Lifecycle(){

};

void Lifecycle::loop(){
 
    if ((CurrentTime - LastMeasureTime) >= Interval)
    {
        pingBroker(); 
        LastMeasureTime = CurrentTime;
    }
    for(auto itr = Lifecycle::array_lifecycle.begin(); itr != Lifecycle::array_lifecycle.end(); ++itr){
        unsigned long CurrenTime = millis();
        if((CurrenTime - Lifecycle::last_time) >= itr->first){
            itr->second();
        }
    }
};

void Lifecycle::add(unsigned long delay,void(*callback)()){
    Lifecycle::array_lifecycle.insert({delay,callback});
}
