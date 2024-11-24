#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cstring> // Для memcmp

const int SAMPLE_RATE = 44100;
const int BITS_PER_SAMPLE = 16;
const int NUM_CHANNELS = 1;
const int BYTES_PER_SAMPLE = BITS_PER_SAMPLE / 8;
const int SILENCE_DURATION = 20;

#pragma pack(push, 1)
struct WAVHeader {
    char chunkID[4];        // "RIFF"
    uint32_t chunkSize;     // Размер файла - 8 байт
    char format[4];         // "WAVE"
};
#pragma pack(pop)

void silenceFirst20Seconds(const std::string& inputFileName, const std::string& outputFileName) {
    std::ifstream inputFile(inputFileName, std::ios::binary);
    if (!inputFile) {
        std::cerr << "Не удалось открыть входной файл: " << inputFileName << std::endl;
        return;
    }

    WAVHeader header;
    inputFile.read(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

    if (std::string(header.chunkID, 4) != "RIFF" || std::string(header.format, 4) != "WAVE") {
        std::cerr << "Неподдерживаемый формат WAV файла." << std::endl;
        return;
    }

    char subchunkID[4];
    uint32_t subchunkSize;

    // Находим подчанк "data"
    while (inputFile.read(reinterpret_cast<char*>(&subchunkID), sizeof(subchunkID))) {
        inputFile.read(reinterpret_cast<char*>(&subchunkSize), sizeof(subchunkSize));

        if (std::string(subchunkID, 4) == "data") {
            break; // Найден подчанк с аудиоданными
        }

        // Пропуск текущего подчанка
        inputFile.seekg(subchunkSize, std::ios::cur);
    }

    if (std::string(subchunkID, 4) != "data") {
        std::cerr << "Подчанк data не найден в файле." << std::endl;
        return;
    }

    std::cout << "Размер аудиоданных: " << subchunkSize << " байт" << std::endl;

    // Читаем аудиоданные
    std::vector<int16_t> audioData(subchunkSize / BYTES_PER_SAMPLE);
    inputFile.read(reinterpret_cast<char*>(audioData.data()), subchunkSize);
    inputFile.close();

    // Заглушаем первые 20 секунд
    int samplesToSilence = SILENCE_DURATION * SAMPLE_RATE;
    for (int i = 0; i < samplesToSilence && i < audioData.size(); ++i) {
        audioData[i] = 0;
    }

    // Записываем результат в выходной файл
    std::ofstream outputFile(outputFileName, std::ios::binary);
    if (!outputFile) {
        std::cerr << "Не удалось открыть выходной файл: " << outputFileName << std::endl;
        return;
    }

    // Перезаписываем заголовок
    header.chunkSize = 4 + (8 + subchunkSize); // Обновляем общий размер файла
    outputFile.write(reinterpret_cast<char*>(&header), sizeof(WAVHeader));

    // Перезаписываем данные
    outputFile.write("fmt ", 4);
    uint32_t fmtChunkSize = 16;
    outputFile.write(reinterpret_cast<char*>(&fmtChunkSize), sizeof(fmtChunkSize));

    uint16_t audioFormat = 1;       // PCM
    uint16_t numChannels = NUM_CHANNELS;
    uint32_t sampleRate = SAMPLE_RATE;
    uint32_t byteRate = SAMPLE_RATE * NUM_CHANNELS * BYTES_PER_SAMPLE;
    uint16_t blockAlign = NUM_CHANNELS * BYTES_PER_SAMPLE;
    uint16_t bitsPerSample = BITS_PER_SAMPLE;

    outputFile.write(reinterpret_cast<char*>(&audioFormat), sizeof(audioFormat));
    outputFile.write(reinterpret_cast<char*>(&numChannels), sizeof(numChannels));
    outputFile.write(reinterpret_cast<char*>(&sampleRate), sizeof(sampleRate));
    outputFile.write(reinterpret_cast<char*>(&byteRate), sizeof(byteRate));
    outputFile.write(reinterpret_cast<char*>(&blockAlign), sizeof(blockAlign));
    outputFile.write(reinterpret_cast<char*>(&bitsPerSample), sizeof(bitsPerSample));

    outputFile.write("data", 4);
    outputFile.write(reinterpret_cast<char*>(&subchunkSize), sizeof(subchunkSize));
    outputFile.write(reinterpret_cast<char*>(audioData.data()), subchunkSize);
    outputFile.close();

    std::cout << "Файл успешно обработан и сохранён как " << outputFileName << std::endl;
}

int main() {
    std::string inputFileName = "input.wav";
    std::string outputFileName = "output.wav";

    silenceFirst20Seconds(inputFileName, outputFileName);

    return 0;
}
