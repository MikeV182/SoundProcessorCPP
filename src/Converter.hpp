#ifndef CONVERTER_H
#define CONVERTER_H

#include "WavFile.hpp"

class Converter {
public:
    virtual void apply(std::vector<int16_t>& samples) = 0;
    virtual ~Converter() = default;
};

#endif
