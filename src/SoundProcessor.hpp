#include <vector>
#include <memory>

#include "Converter.hpp"
#include "WavFile.hpp"

class SoundProcessor {
public:
    SoundProcessor(const std::string& configPath, const std::string& outputFilePath,
                   const std::vector<std::string>& inputFilePaths);

    void process();

private:
    std::vector<std::unique_ptr<Converter>> converters;
    std::vector<WavFile> inputFiles;
    WavFile outputFile;
};
