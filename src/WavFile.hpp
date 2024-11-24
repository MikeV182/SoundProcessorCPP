#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>

const int SAMPLE_RATE = 44100;
const int BITS_PER_SAMPLE = 16;
const int CHANNELS = 1;
const int BYTES_PER_SAMPLE = BITS_PER_SAMPLE / 8;

class WavFile {
public:
    struct WavHeader {
        char chunkID[4];        // "RIFF"
        uint32_t chunkSize;     // Overall filesize - 8 bytes
        char format[4];         // "WAVE"
    };

    WavFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + filename);
        }

        file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));
        
        if (strncmp(header.chunkID, "RIFF", 4) != 0 || strncmp(header.format, "WAVE", 4) != 0) {
            throw std::runtime_error("Unsupported file format: " + filename);
        }

        while (file.read(reinterpret_cast<char*>(&subchunkID), sizeof(subchunkID))) {
            file.read(reinterpret_cast<char*>(&subchunkSize), sizeof(subchunkSize));

            if (strncmp(subchunkID, "data", 4) == 0) {
                break;
            }

            file.seekg(subchunkSize, std::ios::cur);
        }

        if (strncmp(subchunkID, "data", 4) != 0) {
            throw std::runtime_error("Subchunk \'data\' cannot be found");
        }

        std::cout << "Audio data size: " << subchunkSize << " bytes" << std::endl;

        data.resize(subchunkSize / BYTES_PER_SAMPLE);
        file.read(reinterpret_cast<char*>(data.data()), subchunkSize);
        file.close();
    }

    void save(const std::string& filename) {
        std::ofstream outputFile(filename, std::ios::binary);
        if (!outputFile) {
            throw std::runtime_error("Unable to save file: " + filename);
        }

        header.chunkSize = 4 + (8 + subchunkSize); // Обновляем общий размер файла
        outputFile.write(reinterpret_cast<char*>(&header), sizeof(WavHeader));

        outputFile.write("fmt ", 4);
        uint32_t fmtChunkSize = 16;
        outputFile.write(reinterpret_cast<char*>(&fmtChunkSize), sizeof(fmtChunkSize));

        uint16_t audioFormat = 1;
        uint16_t numChannels = CHANNELS;
        uint32_t sampleRate = SAMPLE_RATE;
        uint32_t byteRate = SAMPLE_RATE * CHANNELS * BYTES_PER_SAMPLE;
        uint16_t blockAlign = CHANNELS * BYTES_PER_SAMPLE;
        uint16_t bitsPerSample = BITS_PER_SAMPLE;

        outputFile.write(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
        outputFile.write(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
        outputFile.write(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
        outputFile.write(reinterpret_cast<char*>(&byteRate), sizeof(byteRate));
        outputFile.write(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
        outputFile.write(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));

        outputFile.write("data", 4);
        outputFile.write(reinterpret_cast<char*>(&subchunkSize), sizeof(subchunkSize));
        outputFile.write(reinterpret_cast<char*>(data.data()), subchunkSize);
        outputFile.close();

        std::cout << "File successfully saved as " << filename << std::endl;
    }

    std::vector<int16_t>& getSamples() { return data; }

private:
    WavHeader header;
    char subchunkID[4];
    uint32_t subchunkSize; 
    std::vector<int16_t> data;
};

#endif