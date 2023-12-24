#ifndef CONVERTERS_HPP_
#define CONVERTERS_HPP_

#include <iostream>
#include <vector>

class AbstractConverter {
   public:
    virtual void Convetring(std::vector<std::string> param) = 0;
};

class MuteConverter : public AbstractConverter {
   private:
    std::size_t start;
    std::size_t end;
};

class MixerConverter : public AbstractConverter {
   private:
    std::size_t start;
};

class AddVolumeConverter : public AbstractConverter {
   private:
    std::size_t start;
    std::size_t end;
    short volume;
};

#endif
