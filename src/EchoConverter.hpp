#ifndef ECHO_CONVERTER_H
#define ECHO_CONVERTER_H

#include "Converter.hpp"
#include "WavFile.hpp"

class EchoConverter : public Converter {
    int delaySamples;
    float decay;

public:
    EchoConverter(float delaySec, float decayFactor)
        : delaySamples(static_cast<int>(delaySec * SAMPLE_RATE)), decay(decayFactor) {}

    void apply(std::vector<int16_t>& samples) override {
        for (size_t i = delaySamples; i < samples.size(); ++i) {
            samples[i] += static_cast<int16_t>(samples[i - delaySamples] * decay);
        }
    }
};

#endif
