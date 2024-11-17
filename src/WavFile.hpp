#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cstdint>
#include <cstring>

const int SAMPLE_RATE = 44100;
const int BITS_PER_SAMPLE = 16;
const int CHANNELS = 1;
const int BYTES_PER_SAMPLE = BITS_PER_SAMPLE / 8;

class WavFile {
public:
    struct WavHeader {
        char chunkID[4];
        uint32_t chunkSize;
        char format[4];
        char subchunk1ID[4];
        uint32_t subchunk1Size;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        char subchunk2ID[4];
        uint32_t subchunk2Size;
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

        if (header.audioFormat != 1 || header.numChannels != CHANNELS || header.bitsPerSample != BITS_PER_SAMPLE ||
            header.sampleRate != SAMPLE_RATE) {
            throw std::runtime_error("Unsupported audio format in file: " + filename);
        }

        data.resize(header.subchunk2Size / BYTES_PER_SAMPLE);
        file.read(reinterpret_cast<char*>(data.data()), header.subchunk2Size);
    }

    void save(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to save file: " + filename);
        }

        header.subchunk2Size = data.size() * BYTES_PER_SAMPLE;
        header.chunkSize = 36 + header.subchunk2Size;

        std::cout << "Header - subchunk2Size: " << header.subchunk2Size << std::endl;
        std::cout << "Header - chunkSize: " << header.chunkSize << std::endl;

        file.write(reinterpret_cast<char*>(&header), sizeof(WavHeader));
        file.write(reinterpret_cast<char*>(data.data()), header.subchunk2Size);
    }

    std::vector<int16_t>& getSamples() { return data; }

private:
    WavHeader header;
    std::vector<int16_t> data;
};

#endif
