all:
	cd build && make

1:
	./build/SoundProcessor -c audio_samples/config1.txt audio_samples/output.wav audio_samples/Strange_Days_Are_Here_To_Stay_mono_16bit.wav

2:
	./build/SoundProcessor -c audio_samples/config2.txt audio_samples/output.wav audio_samples/Strange_Days_Are_Here_To_Stay_mono_16bit.wav audio_samples/sugar_honey_ice_tea_mono_16bit.wav

ma:
	g++ createWav.cpp
	./a.out
	rm a.out
	cp output.wav audio_samples/
	rm output.wav

givenSamples:
	./build/SoundProcessor -c audio_samples/config1.txt audio_samples/output.wav audio_samples/given/district_four.wav