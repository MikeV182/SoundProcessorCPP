#include "gtest/gtest.h"
#include "MuteConverter.h"
#include "WavFile.h"

TEST(MuteConverterTest, MuteInterval) {
    WavFile input("test_input.wav");
    WavFile output;
    MuteConverter muteConverter(0, 10);  // заглушить первые 10 секунд
    muteConverter.process(input, output);
    for (int i = 0; i < output.getSampleRate() * 10; ++i) {
        EXPECT_EQ(output.getSamples()[i], 0);
    }
}
