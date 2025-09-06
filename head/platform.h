#pragma once

#include "base.h"
#include "Camera.h"

extern bool is_debug;

class Platform {
public:
    struct CollisionShape{
        float left,right;
        float y;
    };
    CollisionShape shape;
    IMAGE* image=nullptr;
    POINT render_pos={0,0};
    Platform() = default;
    ~Platform() = default;

    void draw(const Camera& camera) const {
        putImage(camera,render_pos.x,render_pos.y,image);
        if(is_debug){
            setlinecolor(RGB(255,0,0));
            line(camera,(int)shape.left,(int)shape.y,(int)shape.right,(int)shape.y);
        }
    }
};