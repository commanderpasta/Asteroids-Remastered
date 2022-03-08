#pragma once

#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <mmsystem.h>
#include <dsound.h>
#include <stdio.h>

#include <string>
#include <vector>

/**
 * A wrapper for creating DirectSound buffers to manage sound for a single window context.
 *
 * Only supports .wav files with 44,100hz sampling, 16-bit and stereo.
 */
class DirectSound
{
private:
	/**
	 * Represents the header block of a .wav file.
	 */
	struct WaveHeaderType
	{
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

	IDirectSound8* m_DirectSound; /** Pointer to an instance of the DirectSound API. */
	IDirectSoundBuffer* m_primaryBuffer; /** Holds a pointer to the primary buffer. The primary buffer acts as the main buffer for all sounds for a window context. */

	void shutdownDirectSound();

public:
	DirectSound();
	DirectSound(const DirectSound& other);
	~DirectSound();

	/**
	 * Represents a sound file to be used with the DirectSound wrapper.
	 * 
	 * Contains its buffer and a unique name to be referenced in the game loop.
	 */
	struct Sound {
		IDirectSoundBuffer8* m_secondaryBuffer;
		std::string fileName;
		std::string name;
		bool isLooping;
	};
	std::vector<Sound> sounds; /** A list that holds all loaded sounds that can be played. */

	bool initialize(HWND hwnd);
	void shutdown();

	bool loadSound(std::string name, std::string fileName);
	bool playSound(Sound&& sound, bool asLoop);
	bool stopSound(Sound&& sound);
	void unloadSoundBuffer(Sound&& sound);
};
