#ifndef TRAFFIC_LIGHT_H_
#define TRAFFIC_LIGHT_H_
#include <iostream>

class TrafficLights {
   public:
    TrafficLights(){};
    void CurrentLight();
    void NextLight();

   private:
    static const char lights[3];
    size_t state = 0;
};

#endif