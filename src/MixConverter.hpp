#ifndef MIX_CONVERTER_H
#define MIX_CONVERTER_H

#include "WavFile.hpp"
#include "Converter.hpp"

class MixConverter : public Converter {
    std::vector<int16_t> additionalSamples;
    int insertAtSample;

public:
    MixConverter(const std::vector<int16_t>& additionalSamples, int insertAt)
        : additionalSamples(additionalSamples), insertAtSample(insertAt * SAMPLE_RATE) {}

    void apply(std::vector<int16_t>& samples) override {
        for (size_t i = 0; i < additionalSamples.size(); ++i) {
            int idx = insertAtSample + i;
            if (idx >= samples.size()) break;
            samples[idx] = static_cast<int16_t>((samples[idx] + additionalSamples[i]) / 2);
        }
    }
};

#endif
