#include <fstream>
#include <iostream>
#include <vector>
#include <cstdint>

const int SAMPLE_RATE = 44100;       // Частота дискретизации
const int BITS_PER_SAMPLE = 16;     // Разрядность
const int CHANNELS = 1;             // Моно
const int BYTES_PER_SAMPLE = BITS_PER_SAMPLE / 8;

// Структура заголовка WAV
struct WavHeader {
    char chunkID[4] = {'R', 'I', 'F', 'F'};       // "RIFF"
    uint32_t chunkSize = 36;                     // Размер файла - 8 байт
    char format[4] = {'W', 'A', 'V', 'E'};       // "WAVE"
    char subchunk1ID[4] = {'f', 'm', 't', ' '};  // "fmt "
    uint32_t subchunk1Size = 16;                 // Размер subchunk1: 16 для PCM
    uint16_t audioFormat = 1;                    // Аудиоформат PCM = 1
    uint16_t numChannels = CHANNELS;             // Количество каналов
    uint32_t sampleRate = SAMPLE_RATE;           // Частота дискретизации
    uint32_t byteRate = SAMPLE_RATE * CHANNELS * BYTES_PER_SAMPLE;  // Байтов в секунду
    uint16_t blockAlign = CHANNELS * BYTES_PER_SAMPLE;              // Байтов на сэмпл
    uint16_t bitsPerSample = BITS_PER_SAMPLE;    // Разрядность
    char subchunk2ID[4] = {'d', 'a', 't', 'a'};  // "data"
    uint32_t subchunk2Size = 0;                  // Размер аудиоданных (0 для пустого файла)
};

// Создание пустого WAV-файла
void createEmptyWav(const std::string& filename) {
    WavHeader header;

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Unable to create file: " + filename);
    }

    // Записываем заголовок
    outFile.write(reinterpret_cast<const char*>(&header), sizeof(header));

    // Закрываем файл
    outFile.close();

    std::cout << "Empty WAV file created: " << filename << "\n";
}

int main() {
    try {
        createEmptyWav("output.wav");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
