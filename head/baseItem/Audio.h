#pragma once
#include <SDL_mixer.h>
#include <SDL.h>
#include <string>

class Audio{
public:
    Audio(){}
    ~Audio(){Mix_FreeChunk(chunk);}
    bool load(const std::string& path){
        chunk = Mix_LoadWAV(path.c_str());
        return chunk != nullptr;
    }
    void play(int loops = 0){
        if (chunk) {
            channel = Mix_PlayChannel(-1, chunk, loops);
        }
    }
    void pause(){
        Mix_Pause(channel);
    }
    void resume(){
        Mix_Resume(channel);
    }
    void stop(){
        Mix_HaltChannel(channel);
    }
private:
    Mix_Chunk* chunk;
    int channel;
};