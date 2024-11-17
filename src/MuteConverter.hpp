#ifndef MUTE_CONVERTER_H
#define MUTE_CONVERTER_H

#include "Converter.hpp"

class MuteConverter : public Converter {
    int startSample, endSample;

public:
    MuteConverter(int start, int end)
        : startSample(start * SAMPLE_RATE), endSample(end * SAMPLE_RATE) {}

    void apply(std::vector<int16_t>& samples) override {
        for (int i = startSample; i < endSample && i < samples.size(); ++i) {
            samples[i] = 0;
        }
    }
};

#endif
