#pragma once

#include "base.h"
#include "Camera.h"
#include "Font.h"
#include "Button.h"
#include <memory>

extern Camera camera;
extern Font font_main;

class MessageBox{
    std::string title;
    std::string message;
    bool isActive = false;
    int boxWidth = 400;
    int boxHeight = 200;
    int boxX = (WINDOW_WIDTH - boxWidth) / 2;
    int boxY = (WINDOW_HEIGHT - boxHeight) / 2;
    int titleHeight=10;
    Button button_close;
public:
    MessageBox(){
        button_close.setPosition({(float)(boxX+boxWidth-30), (float)(boxY+3)});
        button_close.setSize({27,27});
        RenderAction action;
        action.setType(RenderAction::Type::DrawLine);
        action.setColor({0,0,0,255});
        action.setPosition({(float)(boxX+boxWidth-30), (float)(boxY + 30)});
        action.setEndPos({(float)(boxX+boxWidth-3), (float)(boxY+3)});
        button_close.addRenderAction(action);
        action.setPosition({(float)boxX+boxWidth-30,(float)boxY+3});
        action.setEndPos({(float)boxX+boxWidth-3,(float)boxY+30});
        button_close.addRenderAction(action);
        button_close.setCallback(Button::inputResult::Clicked, [this](){
            this->setActive(false);
        });
    }
    void draw(){
        if(!isActive)return;
        SDL_Rect rect = {boxX, boxY, boxWidth, boxHeight};
        camera.setColor({255,255,255,255});
        camera.rect(rect, true);
        camera.setColor({0,0,0,255});
        font_main.drawText(title,Color(150,150,150),{(float)boxX+3, (float)boxY+3});
        font_main.drawText(message, Color(50,50,50),{(float)boxX+5, (float)boxY+boxHeight/2+titleHeight});
        button_close.draw(camera);
    }
    void setMessage(const std::string& msg){message=msg;}
    void setTitle(const std::string& title){this->title=title;}
    void input(const SDL_Event& e){
        if(!isActive)return;
        button_close.input(e);
    }
    void setSize(int width, int height){
        boxWidth=width;
        boxHeight=height;
        boxX = (WINDOW_WIDTH - boxWidth) / 2;
        boxY = (WINDOW_HEIGHT - boxHeight) / 2;
    }
    void setActive(bool active){isActive=active;}
};
extern std::vector<std::shared_ptr<MessageBox>> boxes;