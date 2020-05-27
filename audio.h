#include "audio.h"

Audio::~Audio()
{
    SDL_CloseAudioDevice(deviceid);
    SDL_FreeWAV(wavbuffer);
}

// load file wave vao game
void Audio::load(const char* filename)
{
    SDL_LoadWAV(filename, &waveSpec, &wavbuffer, &wavLength);
    deviceid = SDL_OpenAudioDevice(NULL, 0, &waveSpec, NULL, 0);
}

void Audio::play()
{
    SDL_QueueAudio(deviceid, &wavbuffer, wavLength);
    SDL_PauseAudioDevice(deviceid, 0);
}
