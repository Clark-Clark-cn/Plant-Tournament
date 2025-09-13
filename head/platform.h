#pragma once

#include "base.h"
#include "Camera.h"
#include <nlohmann/json.hpp>

extern bool is_debug;

extern IMAGE img_platform_large;
extern IMAGE img_platform_small;

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
    static Platform fromJson(const nlohmann::json& json){
        Platform platform;
        platform.shape.left = json.value("left", 0.0f);
        platform.shape.right = json.value("right", 0.0f);
        platform.shape.y = json.value("y", 0.0f);
        platform.image = json.value("imageID",0)?&img_platform_large:&img_platform_small;
        platform.render_pos.x = json.value("render_pos_x", 0);
        platform.render_pos.y = json.value("render_pos_y", 0);
        return platform;
    }
    const nlohmann::json& toJson() const {
        static nlohmann::json json;
        json["left"] = shape.left;
        json["right"] = shape.right;
        json["y"] = shape.y;
        json["imageID"] = (image == &img_platform_large) ? 1 : 0;
        json["render_pos_x"] = render_pos.x;
        json["render_pos_y"] = render_pos.y;
        return json;
    }
};