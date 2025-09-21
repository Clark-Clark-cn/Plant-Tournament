#pragma once

#include <functional>

#include "Atlas.h"
#include "Camera.h"
#include "base.h"

class Animation {
    Atlas* atlas = nullptr;
    float interval_ms = 0;
    int frameCount = 0;
    float timer = 0;
    int isLoop = 0;
    double angle = 0.0;
    std::function<void()> onFinished;
    SDL_RendererFlip flip_mode = SDL_FLIP_NONE;
    SDL_FPoint center = { 0.0f,0.0f };
public:
    Animation() = default;
    ~Animation() = default;

    void reset() { timer = 0;frameCount = 0; }
    void setAtlas(Atlas* newAtlas) { reset();atlas = newAtlas; }
    void setLoop(bool loop) { isLoop = loop; }
    void setInterval(int interval) { interval_ms = interval; }
    int getIndexFrame() const { return frameCount; }
    const IMAGE* getFrame()const { return atlas->getImage(frameCount); }
    bool checkFinished() const { if (isLoop)return false;return frameCount >= atlas->size() - 1; }
    void setCallback(std::function<void()> callback) { onFinished = callback; }
    void setFlipMode(SDL_RendererFlip mode) { flip_mode = mode; }
    void setCenter(SDL_FPoint newCenter) { center = newCenter; }

    void update(float delta) {
        timer += delta;
        while (timer >= interval_ms) {
            timer -= interval_ms;
            frameCount++;
            if (frameCount >= atlas->size()) {
                frameCount = isLoop ? 0 : atlas->size() - 1;
                if (!isLoop && onFinished) onFinished();
            }
        }
    }
    void draw(const Camera& camera, int x, int y)const {
        const IMAGE* img = getFrame();
        SDL_FRect dst = { (float)x,(float)y,(float)img->getWidth(),(float)img->getHeight() };
        camera.draw(img, &dst, flip_mode,&center, angle);
    }
};