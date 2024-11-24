#include <iostream>
#include <vector>
#include <unordered_map>

#include "src/WavFile.hpp"
#include "src/ConfigParser.hpp"
#include "src/ConverterFactory.hpp"

int main(int argc, char* argv[]) {
    try {
        if (argc < 5 || std::string(argv[1]) == "-h") {
            std::cout << "Usage: sound_processor -c <config.txt> <output.wav> <input1.wav> [<input2.wav> ...]\n";
            return 0;
        }

        if (std::string(argv[1]) != "-c") {
            throw std::invalid_argument("Invalid arguments. Use -h for help.");
        }

        std::string configFile = argv[2];
        std::string outputFilename = argv[3];

        std::vector<WavFile> inputFiles;
        for (int i = 4; i < argc; ++i) {
            inputFiles.emplace_back(argv[i]);
        }

        ConfigParser config(configFile);
        auto samples = inputFiles[0].getSamples();

        std::unordered_map<int, std::vector<int16_t>> additionalStreams;
        for (size_t i = 1; i < inputFiles.size(); ++i) {
            additionalStreams[i+1] = inputFiles[i].getSamples();
        }

        for (const auto& [name, params] : config.getOperations()) {
            auto converter = ConverterFactory::createConverter(name, params, additionalStreams);
            converter->apply(samples);   
        }

        WavFile output(outputFilename);
        output.getSamples() = samples;
        output.save(outputFilename);

        std::cout << "Processing completed. Output saved to " << outputFilename << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
