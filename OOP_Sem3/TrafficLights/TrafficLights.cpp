#include "TrafficLights.hpp"

const char TrafficLights::lights[3] = {'G', 'Y', 'R'};

void TrafficLights::CurrentLight() {
    std::cout << lights[state] << std::endl;
}

void TrafficLights::NextLight() {
    if (state < 2) {
        state++;
    } else {
        state = 0;
        std::cout << std::endl;
    }
}