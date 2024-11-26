#include "../src/WavFile.hpp"
#include <gtest/gtest.h>
#include <fstream>

void createWavFile(const std::string& filename, const std::vector<int16_t>& samples) {
    std::ofstream file(filename, std::ios::binary);

    file.write("RIFF", 4);
    uint32_t chunkSize = 36 + samples.size() * sizeof(int16_t);
    file.write(reinterpret_cast<char*>(&chunkSize), 4);
    file.write("WAVE", 4);

    file.write("fmt ", 4);
    uint32_t fmtChunkSize = 16;
    file.write(reinterpret_cast<char*>(&fmtChunkSize), 4);
    uint16_t audioFormat = 1;
    uint16_t numChannels = CHANNELS;
    uint32_t sampleRate = SAMPLE_RATE;
    uint32_t byteRate = SAMPLE_RATE * CHANNELS * BYTES_PER_SAMPLE;
    uint16_t blockAlign = CHANNELS * BYTES_PER_SAMPLE;
    uint16_t bitsPerSample = BITS_PER_SAMPLE;
    file.write(reinterpret_cast<char*>(&audioFormat), 2);
    file.write(reinterpret_cast<char*>(&numChannels), 2);
    file.write(reinterpret_cast<char*>(&sampleRate), 4);
    file.write(reinterpret_cast<char*>(&byteRate), 4);
    file.write(reinterpret_cast<char*>(&blockAlign), 2);
    file.write(reinterpret_cast<char*>(&bitsPerSample), 2);

    file.write("data", 4);
    uint32_t subchunkSize = samples.size() * sizeof(int16_t);
    file.write(reinterpret_cast<char*>(&subchunkSize), 4);
    file.write(reinterpret_cast<const char*>(samples.data()), subchunkSize);

    file.close();
}

TEST(WavFileTest, ReadValidFile) {
    std::string filename = "test.wav";
    std::vector<int16_t> samples = {1000, -1000, 500, -500};
    createWavFile(filename, samples);

    WavFile wav(filename);
    auto loadedSamples = wav.getSamples();

    EXPECT_EQ(loadedSamples.size(), samples.size());
    EXPECT_EQ(loadedSamples, samples);

    std::remove(filename.c_str());
}

TEST(WavFileTest, InvalidFileFormat) {
    std::string filename = "invalid.wav";
    std::ofstream file(filename, std::ios::binary);
    file.write("NOTW", 4);
    file.close();

    EXPECT_THROW(WavFile wav(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST(WavFileTest, MissingDataChunk) {
    std::string filename = "missing_data.wav";
    std::ofstream file(filename, std::ios::binary);

    file.write("RIFF", 4);
    uint32_t chunkSize = 36;
    file.write(reinterpret_cast<char*>(&chunkSize), 4);
    file.write("WAVE", 4);

    file.write("fmt ", 4);
    uint32_t fmtChunkSize = 16;
    file.write(reinterpret_cast<char*>(&fmtChunkSize), 4);
    uint16_t audioFormat = 1;
    uint16_t numChannels = CHANNELS;
    uint32_t sampleRate = SAMPLE_RATE;
    uint32_t byteRate = SAMPLE_RATE * CHANNELS * BYTES_PER_SAMPLE;
    uint16_t blockAlign = CHANNELS * BYTES_PER_SAMPLE;
    uint16_t bitsPerSample = BITS_PER_SAMPLE;
    file.write(reinterpret_cast<char*>(&audioFormat), 2);
    file.write(reinterpret_cast<char*>(&numChannels), 2);
    file.write(reinterpret_cast<char*>(&sampleRate), 4);
    file.write(reinterpret_cast<char*>(&byteRate), 4);
    file.write(reinterpret_cast<char*>(&blockAlign), 2);
    file.write(reinterpret_cast<char*>(&bitsPerSample), 2);

    file.close();

    EXPECT_THROW(WavFile wav(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST(WavFileTest, SaveAndValidate) {
    std::string inputFile = "input.wav";
    std::string outputFile = "output.wav";
    std::vector<int16_t> samples = {1000, -1000, 500, -500};
    createWavFile(inputFile, samples);

    WavFile wav(inputFile);
    wav.save(outputFile);

    WavFile savedWav(outputFile);
    auto loadedSamples = savedWav.getSamples();

    EXPECT_EQ(loadedSamples.size(), samples.size());
    EXPECT_EQ(loadedSamples, samples);

    std::remove(inputFile.c_str());
    std::remove(outputFile.c_str());
}

TEST(WavFileTest, EmptyFile) {
    std::string filename = "empty.wav";
    std::ofstream file(filename);
    file.close();

    EXPECT_THROW(WavFile wav(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST(WavFileTest, LargeFile) {
    std::string filename = "large.wav";
    std::vector<int16_t> samples(SAMPLE_RATE * 10, 1000); // 10 секунд аудио
    createWavFile(filename, samples);

    WavFile wav(filename);
    auto loadedSamples = wav.getSamples();

    EXPECT_EQ(loadedSamples.size(), samples.size());
    EXPECT_EQ(loadedSamples, samples);

    std::remove(filename.c_str());
}
