#ifndef CONTROLLER_HPP_
#define CONTROLLER_HPP_

#include <iostream>

class Controller {
   public:
    Controller(){};
    Controller(std::string addr) : ip_addr(addr){};
    ~Controller(){};

   private:
    std::string ip_addr;
};

#endif
