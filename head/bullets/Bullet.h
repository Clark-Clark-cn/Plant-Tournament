#pragma once

#include "../Camera.h"
#include "../Vector2.h"
#include "../Animation.h"
#include "../players/player_id.h"

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
        return (max(position.x + size.x, otherPos.x + otherSize.x) - min(position.x, otherPos.x))
               <= size.x + otherSize.x &&
               (max(position.y + size.y, otherPos.y + otherSize.y) - min(position.y, otherPos.y))
               <= size.y + otherSize.y;
    }
    virtual void update(int delta) {}
    virtual void draw(const Camera& camera) const {
        if(is_debug){
            setfillcolor(RGB(255,255,255));
            setlinecolor(RGB(255,255,255));
            rectangle((int)(position.x), (int)(position.y),
                      (int)(position.x + size.x), (int)(position.y + size.y));
            solidcircle((int)(position.x + size.x / 2), (int)(position.y + size.y / 2), 5);
        }
    }
protected:
    int damage;
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    PlayerID collideTarget;
    bool once=true;
    bool isValid=true;
    bool canRemove=false;
    std::function<void()> callback=nullptr;
    PlayerID targetID=PlayerID::P1;

    bool checkIfExceedsScreen() const {
        return position.x + size.x < 0 || position.x >= getwidth() ||
               position.y + size.y < 0 || position.y >= getheight();
    }
};