#ifndef CONVERTER_FACTORY_H
#define CONVERTER_FACTORY_H

#include "Converter.hpp"
#include "MixConverter.hpp"
#include "EchoConverter.hpp"
#include "MuteConverter.hpp"

#include <memory>
#include <unordered_map>

class ConverterFactory {
public:
    static std::unique_ptr<Converter> createConverter(const std::string& name,
                                                      const std::vector<std::string>& params,
                                                      const std::unordered_map<int, std::vector<int16_t>>& additionalStreams) {
        if (name == "mute") {
            if (params.size() != 2) throw std::invalid_argument("Invalid parameters for mute");
            int start = std::stoi(params[0]);
            int end = std::stoi(params[1]);
            return std::make_unique<MuteConverter>(start, end);
        } else if (name == "mix") {
            if (params.size() != 2 || params[0][0] != '$') throw std::invalid_argument("Invalid parameters for mix");
            int streamIndex = std::stoi(params[0].substr(1));
            int insertAt = std::stoi(params[1]);
            return std::make_unique<MixConverter>(additionalStreams.at(streamIndex), insertAt);
        } else if (name == "echo") {
            if (params.size() != 2) throw std::invalid_argument("Invalid parameters for echo");
            float delay = std::stof(params[0]);
            float decay = std::stof(params[1]);
            return std::make_unique<EchoConverter>(delay, decay);
        }

        throw std::invalid_argument("Unknown converter: " + name);
    }
};

#endif
