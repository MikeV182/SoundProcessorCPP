cmake_minimum_required(VERSION 3.5.0)
project(SoundProcessor VERSION 0.1.0 LANGUAGES C CXX)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/release-1.12.1.zip
)

# Download and add the GoogleTest targets to our build
FetchContent_MakeAvailable(googletest)

# Add the executable for your project
add_executable(SoundProcessor main.cpp)

# Enable C++ 20 (or newer) features
set_target_properties(SoundProcessor PROPERTIES CXX_STANDARD 20 CXX_STANDARD_REQUIRED YES)

# Add test executables
add_executable(config_tests ./test/ConfigParserTests.cpp)
add_executable(converter_tests ./test/ConverterTests.cpp)
add_executable(wav_file_tests ./test/WavFileTests.cpp)

# Link Google Test libraries to the test executables
target_link_libraries(config_tests gtest gtest_main)
target_link_libraries(converter_tests gtest gtest_main)
target_link_libraries(wav_file_tests gtest gtest_main)

# Enable testing support
enable_testing()

# Add the tests
add_test(NAME config_tests COMMAND config_tests)
add_test(NAME converter_tests COMMAND converter_tests)
add_test(NAME wav_file_tests COMMAND wav_file_tests)
