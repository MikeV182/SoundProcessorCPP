#ifndef WAV_FILE_H
#define WAV_FILE_H

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> // Для memcmp
#include <stdexcept>

const int SAMPLE_RATE = 44100;
const int BITS_PER_SAMPLE = 16;
const int CHANNELS = 1;
const int BYTES_PER_SAMPLE = BITS_PER_SAMPLE / 8;

class WavFile {
public:
    struct WavHeader {
        char chunkID[4];        // "RIFF"
        uint32_t chunkSize;     // Общий размер файла - 8 байт
        char format[4];         // "WAVE"
    };

    struct FmtChunk {
        char subchunk1ID[4];    // "fmt "
        uint32_t subchunk1Size; // Размер fmt-чанка
        uint16_t audioFormat;   // Аудиоформат (1 = PCM)
        uint16_t numChannels;   // Количество каналов
        uint32_t sampleRate;    // Частота дискретизации
        uint32_t byteRate;      // Байт/секунда
        uint16_t blockAlign;    // Размер блока
        uint16_t bitsPerSample; // Бит на семпл
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

        FmtChunk fmtChunk{};
        bool foundFmt = false, foundData = false;
        uint32_t dataSize = 0;

        while (file) {
            char subchunkID[4];
            uint32_t subchunkSize;

            file.read(reinterpret_cast<char*>(&subchunkID), sizeof(subchunkID));
            if (!file) break;
            file.read(reinterpret_cast<char*>(&subchunkSize), sizeof(subchunkSize));

            if (strncmp(subchunkID, "fmt ", 4) == 0) {
                file.read(reinterpret_cast<char*>(&fmtChunk), sizeof(FmtChunk) - 8); // -8 из-за subchunkID и subchunkSize
                foundFmt = true;
            } else if (strncmp(subchunkID, "data", 4) == 0) {
                dataSize = subchunkSize;
                data.resize(dataSize / BYTES_PER_SAMPLE);
                file.read(reinterpret_cast<char*>(data.data()), dataSize);
                foundData = true;
            } else {
                file.seekg(subchunkSize, std::ios::cur);
            }
        }

        if (!foundFmt || !foundData) {
            throw std::runtime_error("Required chunks (fmt or data) not found in file: " + filename);
        }

        if (fmtChunk.audioFormat != 1 || fmtChunk.numChannels != CHANNELS ||
            fmtChunk.bitsPerSample != BITS_PER_SAMPLE || fmtChunk.sampleRate != SAMPLE_RATE) {
            throw std::runtime_error("Unsupported audio format in file: " + filename);
        }

        this->fmtChunk = fmtChunk;
        this->dataSize = dataSize;
    }

    void save(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to save file: " + filename);
        }

        uint32_t newDataSize = data.size() * BYTES_PER_SAMPLE;
        uint32_t newChunkSize = 36 + newDataSize;

        WavHeader newHeader = header;
        newHeader.chunkSize = newChunkSize;
        file.write(reinterpret_cast<char*>(&newHeader), sizeof(WavHeader));

        FmtChunk newFmtChunk = fmtChunk;
        file.write("fmt ", 4);
        file.write(reinterpret_cast<char*>(&newFmtChunk.subchunk1Size), sizeof(newFmtChunk.subchunk1Size));
        file.write(reinterpret_cast<char*>(&newFmtChunk.audioFormat), sizeof(FmtChunk) - 8); // -8 из-за subchunkID и subchunkSize

        file.write("data", 4);
        file.write(reinterpret_cast<char*>(&newDataSize), sizeof(newDataSize));
        file.write(reinterpret_cast<char*>(data.data()), newDataSize);

        std::cout << "File successfully saved as " << filename << std::endl;
    }

    std::vector<int16_t>& getSamples() { return data; }

private:
    WavHeader header;
    FmtChunk fmtChunk;
    uint32_t dataSize;
    std::vector<int16_t> data;
};

#endif