#include "../src/MuteConverter.hpp"
#include "../src/MixConverter.hpp"
#include "../src/EchoConverter.hpp"
#include "../src/ConverterFactory.hpp"
#include <gtest/gtest.h>
#include <unordered_map>

#define SAMPLE_RATE 44100

// Тест для MuteConverter
TEST(MuteConverterTest, ApplyMute) {
    std::vector<int16_t> samples(SAMPLE_RATE * 2, 1000); // 2 секунды аудио
    MuteConverter mute(1, 2); // Затухание с 1-й по 2-ю секунду
    mute.apply(samples);

    // Проверяем, что затухание произошло только на заданном диапазоне
    for (int i = 0; i < SAMPLE_RATE; ++i) {
        EXPECT_EQ(samples[i], 1000); // До затухания
    }
    for (int i = SAMPLE_RATE; i < 2 * SAMPLE_RATE; ++i) {
        EXPECT_EQ(samples[i], 0); // Затухание
    }
}

// Тест для MixConverter
TEST(MixConverterTest, ApplyMix) {
    std::vector<int16_t> samples(SAMPLE_RATE, 2000); // 1 секунда базового аудио
    std::vector<int16_t> additionalSamples(SAMPLE_RATE / 2, 4000); // 0.5 секунды доп. аудио
    MixConverter mix(additionalSamples, 0); // Микс в начало
    mix.apply(samples);

    // Проверяем, что первые 0.5 секунды аудио смешаны
    for (size_t i = 0; i < additionalSamples.size(); ++i) {
        EXPECT_EQ(samples[i], (2000 + 4000) / 2);
    }
    // Остальная часть должна остаться неизменной
    for (size_t i = additionalSamples.size(); i < samples.size(); ++i) {
        EXPECT_EQ(samples[i], 2000);
    }
}

// Тест для EchoConverter
TEST(EchoConverterTest, ApplyEcho) {
    std::vector<int16_t> samples(SAMPLE_RATE, 1000); // 1 секунда аудио
    EchoConverter echo(0.5, 0.5); // Эхо с задержкой 0.5 секунды и затуханием 50%
    echo.apply(samples);

    // Проверяем, что первые 0.5 секунды остались неизменными
    for (size_t i = 0; i < SAMPLE_RATE / 2; ++i) {
        EXPECT_EQ(samples[i], 1000);
    }
    // Проверяем эхо после 0.5 секунды
    for (size_t i = SAMPLE_RATE / 2; i < SAMPLE_RATE; ++i) {
        EXPECT_EQ(samples[i], 1000 + static_cast<int16_t>(1000 * 0.5)); // Эхо добавляет 50%
    }
}

// Тест для ConverterFactory
TEST(ConverterFactoryTest, CreateMuteConverter) {
    auto converter = ConverterFactory::createConverter("mute", {"1", "2"}, {});
    std::vector<int16_t> samples(SAMPLE_RATE * 2, 1000); // 2 секунды аудио
    converter->apply(samples);

    // Проверяем, что затухание произошло только на заданном диапазоне
    for (int i = 0; i < SAMPLE_RATE; ++i) {
        EXPECT_EQ(samples[i], 1000); // До затухания
    }
    for (int i = SAMPLE_RATE; i < 2 * SAMPLE_RATE; ++i) {
        EXPECT_EQ(samples[i], 0); // Затухание
    }
}

TEST(ConverterFactoryTest, CreateMixConverter) {
    std::unordered_map<int, std::vector<int16_t>> additionalStreams = {
        {0, std::vector<int16_t>(SAMPLE_RATE / 2, 4000)} // 0.5 секунды доп. аудио
    };
    auto converter = ConverterFactory::createConverter("mix", {"$0", "0"}, additionalStreams);
    std::vector<int16_t> samples(SAMPLE_RATE, 2000); // 1 секунда базового аудио
    converter->apply(samples);

    // Проверяем, что первые 0.5 секунды аудио смешаны
    for (size_t i = 0; i < additionalStreams[0].size(); ++i) {
        EXPECT_EQ(samples[i], (2000 + 4000) / 2);
    }
    // Остальная часть должна остаться неизменной
    for (size_t i = additionalStreams[0].size(); i < samples.size(); ++i) {
        EXPECT_EQ(samples[i], 2000);
    }
}

TEST(ConverterFactoryTest, CreateEchoConverter) {
    auto converter = ConverterFactory::createConverter("echo", {"0.5", "0.5"}, {});
    std::vector<int16_t> samples(SAMPLE_RATE, 1000); // 1 секунда аудио
    converter->apply(samples);

    // Проверяем, что первые 0.5 секунды остались неизменными
    for (size_t i = 0; i < SAMPLE_RATE / 2; ++i) {
        EXPECT_EQ(samples[i], 1000);
    }
    // Проверяем эхо после 0.5 секунды
    for (size_t i = SAMPLE_RATE / 2; i < SAMPLE_RATE; ++i) {
        EXPECT_EQ(samples[i], 1000 + static_cast<int16_t>(1000 * 0.5)); // Эхо добавляет 50%
    }
}

TEST(ConverterFactoryTest, InvalidConverterName) {
    EXPECT_THROW(ConverterFactory::createConverter("unknown", {}, {}), std::invalid_argument);
}

TEST(ConverterFactoryTest, InvalidMuteParams) {
    EXPECT_THROW(ConverterFactory::createConverter("mute", {"1"}, {}), std::invalid_argument);
}

TEST(ConverterFactoryTest, InvalidMixParams) {
    EXPECT_THROW(ConverterFactory::createConverter("mix", {"0"}, {}), std::invalid_argument);
}

TEST(ConverterFactoryTest, InvalidEchoParams) {
    EXPECT_THROW(ConverterFactory::createConverter("echo", {"0.5"}, {}), std::invalid_argument);
}

