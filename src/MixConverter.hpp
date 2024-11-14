#include "WavFile.hpp"
#include "Converter.hpp"

class MixConverter : public Converter {
public:
    MixConverter(WavFile& additionalStream, int insertTime = 0);

    void process(WavFile& input, WavFile& output) override;

private:
    WavFile& additionalStream;
    int insertTime;
};
