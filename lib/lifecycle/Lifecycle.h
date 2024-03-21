#ifndef LIFECYCLE_H
#define LIFECYCLE_H
#include <map>
#include <iostream>
using namespace std; 

struct Element {
    int value;
    void (*function)();
};

class Lifecycle {
    public: 
        Lifecycle();
        static void loop();
        static void add(unsigned long delay,void(*callback)());
    private: 
        static std::map<unsigned long,void(*)()> array_lifecycle;
        static unsigned long last_time;
    };

#endif