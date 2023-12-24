#ifndef CONVERT_FACTORY_HPP_
#define CONVERT_FACTORY_HPP_

#include "Converters.hpp"
#include "Exeption.hpp"

class ConvertFactory {
   public:
    AbstractConverter* CreateConverter(const std::vector<std::string>& param) {
        if (param.size() == 0) {
            throw SoundProcessorException("Parameters are missing");
        }
        if (param[0] == "mute") {
            return new MuteConverter(param);
        } else if (param[0] == "mix") {
            return new MixerConverter(param);
        } else if (param[0] == "addvol") {
            return new AddVolumeConverter(param);
        } else {
            throw SoundProcessorException("Invalid Converter Parameter");
        }
    }
};

#endif  // CONVERT_FACTORY_HPP_
