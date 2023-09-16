#include "TrafficLights.hpp"

int main() {
    TrafficLights TL;
    for (std::size_t i = 0; i < 10; ++i) {
        TL.NextLight();
        TL.CurrentLight();
    }
    return 0;
}