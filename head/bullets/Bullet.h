#pragma once

#include "baseItem/Camera.h"
#include "baseItem/Vector2.h"
#include "baseItem/Animation.h"
#include "players/player_id.h"

#include <functional>

extern bool is_debug;

class Bullet{
public:
    Bullet()=default;
    ~Bullet()=default;

    void setDamage(int damage){
        this->damage=damage;
    }
    int getDamage() const {
        return damage;
    }
    void setPosition(Vector2 position) {
        this->position = position;
    }
    Vector2 getPosition() const {
        return position;
    }
    void setVelocity(Vector2 velocity) {
        this->velocity = velocity;
    }
    Vector2 getVelocity() const {
        return velocity;
    }
    const Vector2& getSize() const {
        return size;
    }
    void setSize(const Vector2& size) {
        this->size = size;
    }
    void setCollideTarget(PlayerID id) {
        collideTarget = id;
    }
    void setCallback(std::function<void()> callback) {
        this->callback = callback;
    }
    PlayerID getCollideTarget() const {
        return collideTarget;
    }
    void setValid(bool valid) {
        isValid = valid;
    }
    bool getValid() const {
        return isValid;
    }
    bool checkCanRemove() const {
        return canRemove;
    }
    virtual void collide(){
        if(once)isValid=false;
        if (callback) {
            callback();
        }
    }
    virtual bool checkCollision(const Vector2& otherPos, const Vector2& otherSize) {
        return (std::max(position.x + size.x, otherPos.x + otherSize.x) - std::min(position.x, otherPos.x))
               <= size.x + otherSize.x &&
               (std::max(position.y + size.y, otherPos.y + otherSize.y) - std::min(position.y, otherPos.y))
               <= size.y + otherSize.y;
    }
    virtual void update(float delta) {}
    virtual void draw(const Camera& camera) const {
        if(is_debug){
            camera.setColor(Color{255,255,255,255});
            camera.rect({(int)position.x, (int)position.y, (int)size.x, (int)size.y});
            camera.circle(Vector2{position.x + size.x / 2, position.y + size.y / 2}, 5,true);
        }
    }
protected:
    int damage;
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    PlayerID collideTarget=PlayerID::P1;
    bool once=true;
    bool isValid=true;
    bool canRemove=false;
    std::function<void()> callback=nullptr;

    bool checkIfExceedsScreen() const {
        return position.x + size.x < -1000 || position.x >= (WINDOW_WIDTH+1000) ||
               position.y + size.y < -1000 || position.y >= (WINDOW_HEIGHT+1000);
    }
};