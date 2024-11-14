#include "Converter.hpp"
#include "WavFile.hpp"

class MuteConverter : public Converter {
public:
    MuteConverter(int start, int end) : start_(start), end_(end) {}

    void process(WavFile& input, WavFile& output) override {
        int startSample = start_ * input.getSampleRate();
        int endSample = end_ * input.getSampleRate();
        for (int i = startSample; i < endSample; ++i) {
            output.getSamples()[i] = 0;
        }
    }

private:
    int start_;
    int end_;
};

