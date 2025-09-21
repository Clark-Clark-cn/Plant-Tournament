#pragma once

#include "Image.h"
#include "Vector2.h"
#include "base.h"
#include "Camera.h"
#include "Font.h"
#include <SDL.h>
#include <functional>

class RenderAction{
public:
    enum class Type{
        None,
        DrawImage,
        DrawRect,
        DrawLine,
        DrawText
    };
private:
    Type type=Type::None;
    Color color={255,255,255,255};
    Vector2 position={0,0};
    Vector2 size={0,0};
    Vector2 endPos={0,0};
    IMAGE* image=nullptr;
    std::string text;
    SDL_RendererFlip flip=SDL_FLIP_NONE;
    bool filled=false;
public:
    RenderAction()=default;
    ~RenderAction()=default;
    void setType(Type type){this->type=type;}
    Type getType() const {return type;}
    void setColor(const Color& color){this->color=color;}
    Color getColor() const {return color;}
    void setPosition(const Vector2& pos){this->position=pos;}
    Vector2 getPosition() const {return position;}
    void setSize(const Vector2& size){this->size=size;}
    Vector2 getSize() const {return size;}
    void setEndPos(const Vector2& pos){this->endPos=pos;}
    Vector2 getEndPos() const {return endPos;}
    void setImage(IMAGE* img){this->image=img;}
    IMAGE* getImage() const {return image;}
    void setText(const std::string& text){this->text=text;}
    std::string getText() const {return text;}
    void setFlip(SDL_RendererFlip flip){this->flip=flip;}
    SDL_RendererFlip getFlip() const {return flip;}

    void clear(){
        type=Type::None;
        color={255,255,255,255};
        position={0,0};
        size={0,0};
        endPos={0,0};
        image=nullptr;
        text="";
    }
    void draw(const Camera& camera)const{
        switch(type){
            case Type::DrawImage:
                if(image)camera.draw(position,image,flip);
                break;
            case Type::DrawRect:
                camera.setColor(color);
                camera.rect({(int)position.x,(int)position.y,(int)size.x,(int)size.y},filled);
                break;
            case Type::DrawLine:
                camera.setColor(color);
                camera.line(position,endPos);
                break;
            case Type::DrawText:
                font_main.drawText(text,color,position);
                break;
            default:
                break;
        }
    }
};

class Button{
public:
    enum class inputResult{
        None,
        Clicked,
        Released
    };
private:
    Vector2 position={0,0};
    Vector2 size={0,0};
    std::unordered_map<std::string,std::vector<RenderAction>> actions;
    std::vector<RenderAction> *actions_active=nullptr;
    std::unordered_map<inputResult,std::function<void()>> callbacks;
public:
    Button()=default;
    ~Button()=default;

    void setPosition(const Vector2& pos){this->position=pos;}
    Vector2 getPosition() const {return position;}
    void setSize(const Vector2& size){this->size=size;}
    Vector2 getSize() const {return size;}
    void setRenderActions(const std::vector<RenderAction>& actions, const std::string id="default"){this->actions[id]=actions;}
    std::vector<RenderAction>& getRenderActions(const std::string id="default"){return actions[id];}
    void addRenderAction(const RenderAction& action, const std::string id="default"){actions[id].push_back(action);}
    void clearRenderActions(const std::string id="default"){actions[id].clear();}
    void setActiveRenderActionSet(const std::string id="default"){
        if(actions.count(id)){
            actions_active=&actions[id];
        }
    }
    void setCallback(inputResult result, std::function<void()> callback){callbacks[result]=callback;}
    void addItem(const IMAGE* img, SDL_RendererFlip flip, const Vector2& pos={0,0}, const std::string id="default"){
        Vector2 rel_pos=pos;
        if(pos==Vector2(0,0))rel_pos=position;
        RenderAction action;
        action.setType(RenderAction::Type::DrawImage);
        action.setPosition(rel_pos);
        action.setImage((IMAGE*)img);
        action.setFlip(flip);
        actions[id].push_back(action);
        size={(float)img->getSize().w,(float)img->getSize().h};
        position=rel_pos;
    }

    void draw(const Camera& camera) const {
        if(!actions_active)return;
        for(const auto& action : *actions_active){
            action.draw(camera);
        }
        if(is_debug){
            camera.setColor(Color(255,0,0,255));
            camera.rect({(int)position.x,(int)position.y,(int)size.x,(int)size.y},false);
        }
    }
    bool input(const SDL_Event& e){
        switch(e.type){
        case SDL_MOUSEBUTTONDOWN:{
            int mx=e.button.x;
            int my=e.button.y;
            if(mx>=position.x&&mx<=position.x+size.x&&my>=position.y&&my<=position.y+size.y){
                if(callbacks.count(inputResult::Clicked)){
                    callbacks[inputResult::Clicked]();
                }
                return true;
            }
        }
        break;
        case SDL_MOUSEBUTTONUP:{
            int mx=e.button.x;
            int my=e.button.y;
            if(mx>=position.x&&mx<=position.x+size.x&&my>=position.y&&my<=position.y+size.y){
                if(callbacks.count(inputResult::Released)){
                    callbacks[inputResult::Released]();
                }
                return true;
            }
        }
        break;
        }
        return false;
    }

};