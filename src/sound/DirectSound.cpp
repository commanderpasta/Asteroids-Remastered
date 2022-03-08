#include "DirectSound.h"

/**
 * Creates an instance of the DirectSound API.
 */
DirectSound::DirectSound()
{
	m_DirectSound = 0;
	m_primaryBuffer = 0;
}


DirectSound::DirectSound(const DirectSound& other)
{
}


DirectSound::~DirectSound()
{
}

/**
 * Initializes the DirectSound with the default audio device.
 * 
 * Formats the primary buffer for .WAV files with 44,100Hz sample rate in 16-bit stereo.
 * 
 * \param hwnd The window context to use DirectSound with.
 * \return Whether the initialization was successful.
 */
bool DirectSound::initialize(HWND hwnd)
{
	HRESULT result;
	DSBUFFERDESC bufferDesc;
	WAVEFORMATEX waveFormat;

	// Initialize DirectSound for the default sound device.
	result = DirectSoundCreate8(NULL, &m_DirectSound, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Set the cooperative level to priority so the format of the primary sound buffer can be modified.
	result = m_DirectSound->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the primary buffer.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = 0;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = NULL;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Get control of the primary sound buffer on the default sound device.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &m_primaryBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Setup the format of the primary sound bufffer.
	// It is set up for .WAV files recorded with a 44,100Hz sample rate in 16-bit stereo.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the primary buffer to the specified wave format.
	result = m_primaryBuffer->SetFormat(&waveFormat);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/**
 * Begins shutdown of the DirectSound API instance.
 */
void DirectSound::shutdown()
{
	// Release the secondary buffer.
	for (Sound sound : this->sounds) {
		unloadSoundBuffer(std::move(sound));
	}

	shutdownDirectSound();
}

/**
 * Releases the primary buffer and DirectSound interface pointers.
 */
void DirectSound::shutdownDirectSound()
{
	if (m_primaryBuffer)
	{
		m_primaryBuffer->Release();
		m_primaryBuffer = 0;
	}

	if (m_DirectSound)
	{
		m_DirectSound->Release();
		m_DirectSound = 0;
	}
}

/**
 * Loads a new sound from a file.
 * 
 * Checks the file header against the API specification for .WAV files
 * and assigns buffers and a <Sound> object to it.
 * 
 * \param name The name to use as a reference for the sound.
 * \param fileName The file path of the sound.
 * \return Returns a bool whether the sound was loaded succesfully.
 */
bool DirectSound::loadSound(std::string name, std::string fileName)
{
	int error;
	FILE* filePtr;
	unsigned int count;
	WaveHeaderType waveFileHeader;
	WAVEFORMATEX waveFormat;
	DSBUFFERDESC bufferDesc;
	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	IDirectSoundBuffer8* secondaryBuffer = 0;
	unsigned char* waveData;
	unsigned char* bufferPtr;
	unsigned long bufferSize;

	const char* fileNameAsChar = fileName.c_str();

	// Open the wave file in binary.
	error = fopen_s(&filePtr, fileNameAsChar, "rb");
	if (error != 0)
	{
		return false;
	}

	// Read in the wave file header.
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1)
	{
		return false;
	}

	//BEGIN CHECK FORMAT

	// Check that the chunk ID is the RIFF format.
	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// Check that the sub chunk ID is the fmt format.
	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' '))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM.
	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM)
	{
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (waveFileHeader.numChannels != 2)
	{
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (waveFileHeader.sampleRate != 44100)
	{
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (waveFileHeader.bitsPerSample != 16)
	{
		return false;
	}

	// Check for the data chunk header.
	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a'))
	{
		return false;
	}
	
	//END CHECK FORMAT

	// Set the wave format of secondary buffer that this wave file will be loaded onto.
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	// Set the buffer description of the secondary sound buffer that the wave file will be loaded onto.
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	// Create a temporary sound buffer with the specific buffer settings.
	result = m_DirectSound->CreateSoundBuffer(&bufferDesc, &tempBuffer, NULL);
	if (FAILED(result))
	{
		return false;
	}

	// Test the buffer format against the DirectSound8 interface and create the secondary buffer.
	result = tempBuffer->QueryInterface(IID_IDirectSoundBuffer8, (void**)&secondaryBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the temporary buffer.
	tempBuffer->Release();
	tempBuffer = 0;

	// Move to the beginning of the wave data which starts at the end of the data chunk header.
	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	// Create a temporary buffer to hold the wave file data.
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData)
	{
		return false;
	}

	// Read in the wave file data into the newly created buffer.
	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize)
	{
		return false;
	}

	// Close the file once done reading.
	error = fclose(filePtr);
	if (error != 0)
	{
		return false;
	}

	// Lock the secondary buffer to write data into it.
	result = secondaryBuffer->Lock(0, waveFileHeader.dataSize, (void**)&bufferPtr, (DWORD*)&bufferSize, NULL, 0, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Copy the data into the buffer.
	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	// Unlock the secondary buffer after the data has been written to it.
	result = secondaryBuffer->Unlock((void*)bufferPtr, bufferSize, NULL, 0);
	if (FAILED(result))
	{
		return false;
	}

	// Release the wave data since it was copied into the secondary buffer.
	delete[] waveData;
	waveData = 0;


	// Add new buffer to list
	Sound newSound = { std::move(secondaryBuffer), fileName, name, false };
	this->sounds.push_back(std::move(newSound));

	return true;
}

/**
 * Unloads the secondary buffer of a managed sound.
 * 
 * \param sound The sound to unload the buffer for.
 */
void DirectSound::unloadSoundBuffer(Sound&& sound)
{
	if (sound.m_secondaryBuffer)
	{
		sound.m_secondaryBuffer->Release();
		sound.m_secondaryBuffer = 0;
	}
}

/**
 * Plays a loaded <Sound>.
 * 
 * Can be played in an endless loop until it is stopped manually.
 * 
 * @see stopSound
 * 
 * \param sound The <Sound> to play.
 * \param asLoop A bool that says whether the sound should play in a loop. The loop can be stopped manually.
 * \return A bool that says whether the sound has been played successfully.
 */
bool DirectSound::playSound(Sound&& sound, bool asLoop)
{
	HRESULT result;

	// Set position at the beginning of the sound buffer.
	result = sound.m_secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result))
	{
		return false;
	}

	//Plays the sound at ~10db below 100%
	result = sound.m_secondaryBuffer->SetVolume(-1000); 
	if (FAILED(result))
	{
		return false;
	}

	// Play the contents of the secondary sound buffer.
	if (asLoop) {
		result = sound.m_secondaryBuffer->Play(0, 0, DSBPLAY_LOOPING); //DSBPLAY_LOOPING for loop
	} else {
		result = sound.m_secondaryBuffer->Play(0, 0, 0);
	}

	if (FAILED(result))
	{
		return false;
	}

	sound.isLooping = true;
	return true;

}

/**
 * Stops a sound from playing.
 * 
 * Can be used to stop sounds that are playing in a loop.
 * 
 * \param sound The sound that should be stopped.
 * \return A bool that says whether the sound has been stopped successfully.
 */
bool DirectSound::stopSound(Sound&& sound)
{
	HRESULT result;

	// Set position at the beginning of the sound buffer.
	result = sound.m_secondaryBuffer->Stop();

	if (FAILED(result))
	{
		return false;
	}

	sound.isLooping = false;
	return true;
}