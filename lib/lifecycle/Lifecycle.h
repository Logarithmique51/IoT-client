#ifndef LIFECYCLE_H
#define LIFECYCLE_H
#include <vector>

struct Event {
  unsigned int delay;
  unsigned int elapsedTime;
  void (*fn)();
};

class Lifecycle {
    public: 
        static void loop();
        static void add(unsigned long delay,void(*callback)());
    private: 
        static std::vector<Event> events;
    };

#endif