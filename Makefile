all:
	cd build && make

1:
	cd build && ./SoundProcessor -c ../audio_samples/config1.txt output.wav input1.wav 

2:
	cd build && ./SoundProcessor -c ../audio_samples/config2.txt output.wav input1.wav input2.wav 

3:
	cd build && ./SoundProcessor -c ../audio_samples/config3.txt output.wav input2.wav 

ma:
	g++ createWav.cpp
	./a.out
	rm a.out
	cp output.wav build/
	rm output.wav

givenSamples:
	./build/SoundProcessor -c audio_samples/config1.txt audio_samples/output.wav audio_samples/given/district_four.wav