# CMake generated Testfile for 
# Source directory: /home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP
# Build directory: /home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/build
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(config_tests "/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/build/config_tests")
set_tests_properties(config_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/CMakeLists.txt;34;add_test;/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/CMakeLists.txt;0;")
add_test(converter_tests "/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/build/converter_tests")
set_tests_properties(converter_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/CMakeLists.txt;35;add_test;/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/CMakeLists.txt;0;")
add_test(wav_file_tests "/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/build/wav_file_tests")
set_tests_properties(wav_file_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/CMakeLists.txt;36;add_test;/home/mike/Documents/Cpp/lectTask3/SoundProcessorCPP/CMakeLists.txt;0;")
subdirs("_deps/googletest-build")
