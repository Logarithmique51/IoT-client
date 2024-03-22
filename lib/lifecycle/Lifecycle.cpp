#include "Lifecycle.h"
#include <ESP8266WiFi.h>
#include <vector>

std::vector<Event> Lifecycle::events ;

void Lifecycle::loop(){
    for(Event &event : events){
        unsigned long currentTime = millis();
        if((currentTime - event.elapsedTime) >= event.delay){
            event.fn();
            event.elapsedTime = currentTime;
        }
    }
};

void Lifecycle::add(unsigned long delay,void(*callback)()){
    events.push_back({delay,millis(),callback});
}
