#include <iostream>
#include "src/SoundProcessor.hpp"

void printHelp() {
    std::cout << "Usage: sound_processor -c <config.txt> <output.wav> <input1.wav> [<input2.wav> ...]" << std::endl;
    // TODO: Добавить описание ковертеров
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printHelp();
        return 1;
    }

    std::string configPath;
    std::string outputFilePath;
    std::vector<std::string> inputFilePaths;

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-h") {
            printHelp();
            return 0;
        } else if (arg == "-c") {
            if (i + 1 < argc) {
                configPath = argv[++i];
            } else {
                std::cerr << "Error: Missing argument for -c" << std::endl;
                return 1;
            }
        } else if (outputFilePath.empty()) {
            outputFilePath = arg;
        } else {
            inputFilePaths.push_back(arg);
        }
    }

    try {
        SoundProcessor processor(configPath, outputFilePath, inputFilePaths);
        processor.process();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
