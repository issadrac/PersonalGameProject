#pragma once
class Audio {
public:
    SDL_AudioSpec spec;
    Uint8* wav_data = NULL;
    Uint32 wav_len = 0;
    SDL_AudioStream* levelStream = nullptr;
    SDL_AudioStream* soundStream = nullptr;
    SDL_AudioStream* winningStream = nullptr;
    void playLevelWav(int cl) {
        string filename = "level" + to_string(cl) + "sound";
        filename = "Audio\\" + filename + ".wav";
        if (!SDL_LoadWAV(filename.c_str(), &spec, &wav_data, &wav_len)) {
            SDL_Log("Couldn't load WAV: %s", SDL_GetError());
        }
        if (!levelStream) {
            levelStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
            SDL_ResumeAudioStreamDevice(levelStream);
        }
        if (!levelStream) {
            SDL_Log("Failed to open audio stream: %s", SDL_GetError());
            SDL_free(wav_data);
        }
        
        SDL_PutAudioStreamData(levelStream, wav_data, wav_len);
    }
    void playWinningWav(string filename) {
        filename = "Audio\\" + filename + ".wav";
        if (!SDL_LoadWAV(filename.c_str(), &spec, &wav_data, &wav_len)) {
            SDL_Log("Couldn't load WAV: %s", SDL_GetError());
        }
        if (!winningStream) {
            winningStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
            SDL_ResumeAudioStreamDevice(winningStream);
        }
        if (!winningStream) {
            SDL_Log("Failed to open audio stream: %s", SDL_GetError());
            SDL_free(wav_data);
        }
        SDL_PutAudioStreamData(winningStream, wav_data, wav_len);
    }
    void playSomething(string filename) {
        filename = "Audio\\" + filename + ".wav";
        if (!SDL_LoadWAV(filename.c_str(), &spec, &wav_data, &wav_len)) {
            SDL_Log("Couldn't load WAV: %s", SDL_GetError());
        }
        if (!soundStream) {
            soundStream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &spec, NULL, NULL);
            SDL_ResumeAudioStreamDevice(soundStream);
        }
        if (!soundStream) {
            SDL_Log("Failed to open audio stream: %s", SDL_GetError());
            SDL_free(wav_data);
        }  
        SDL_PutAudioStreamData(soundStream, wav_data, wav_len);
    }
    void stopWav() {
        SDL_PauseAudioStreamDevice(levelStream);
    }
    ~Audio() {
        SDL_DestroyAudioStream(soundStream);
        SDL_DestroyAudioStream(levelStream);
    }
};