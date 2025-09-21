#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include "base.h"

class Atlas
{
    std::vector<IMAGE> img_list;
public:
    Atlas() = default;
    ~Atlas() = default;

    void loadimage(SDL_Renderer* renderer, const std::string& filePath, int num) {
        img_list.clear();
        img_list.resize(num);

        char path[256];
        for (int i = 0; i < num; ++i) {
            snprintf(path, sizeof(path), filePath.c_str(), i + 1);
            img_list[i].load(renderer,path);
        }
    }
    void clear() {
        img_list.clear();
    }
    int size() const {
        return img_list.size();
    }
    const IMAGE* getImage(int index) {
        if (index < 0 || index >= img_list.size()) return &img_list.back();
        return &img_list[index];
    }
    void addImage(const IMAGE& img) {
        img_list.push_back(img);
    }
};