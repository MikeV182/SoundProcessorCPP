#include <vector>
#include <string>
#include <stdexcept>

class WavFile {
public:
    WavFile(const std::string& filename);
    ~WavFile();

    void load();
    void save(const std::string& filename);
    int16_t* getSamples();
    int getSampleRate() const;
    int getNumSamples() const;

private:
    std::string filename;
    int16_t* samples;
    int sampleRate;
    int numSamples;

    void validateFormat();
};
