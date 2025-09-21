#pragma once

#include "baseItem/base.h"
#include "baseItem/Camera.h"
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
    IMAGE* image;
    Vector2 render_pos={0,0};
    Platform() = default;
    ~Platform() = default;

    void draw(const Camera& camera) const {
        camera.draw(render_pos,image);
        if(is_debug){
            camera.setColor(Color(255,0,0,255));
            camera.line({shape.left,shape.y},{shape.right,shape.y});
        }
    }
    static Platform fromJson(const nlohmann::json& json){
        Platform platform;
        platform.image = json.value("imageID",0)?&img_platform_large:&img_platform_small;
        platform.render_pos.x = json.value("render_pos_x", 0);
        platform.render_pos.y = json.value("render_pos_y", 0);

        float left = json.value("left", 0.0f);
        float right = json.value("right", 0.0f);
        float y = json.value("y", 0.0f);

        if(right <= left||right-left<10.0f||y<0.0f){
            const int w = platform.image->getWidth();
            const int h = platform.image->getHeight();
            const int margin = platform.image==&img_platform_large?30:40;
            platform.shape.left = platform.render_pos.x + margin;
            platform.shape.right = platform.render_pos.x + (w > margin ? (w - margin) : 0);
            platform.shape.y = platform.render_pos.y + ((platform.image == &img_platform_large) ? 60 : (h / 2));
        }
        else{
            platform.shape.left = left;
            platform.shape.right = right;
            platform.shape.y = y;
        }
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