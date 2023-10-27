#include "Base.hpp"

int main() {
    std::list<Base*> sequence;
    // sequence.push_back(new A);
    sequence.push_back(new B);
    for (std::size_t i = 0; i < 10; ++i) {
        for (std::list<Base*>::iterator it = sequence.begin(); it != sequence.end(); ++it) {
            (*it)->change(sequence, it);
        }
        for (auto const& el : sequence) {
            std::cout << el->getCondition();
        }
        std::cout << std::endl;
    }
    for (auto const& el : sequence) {
        delete el;
    }
    sequence.clear();
}