#pragma once
#include "screen.h"

#include "manager.h"
#include <iostream>
#include "baseItem/MessageBox.h"

extern ScreenManager screenManager;

extern IMAGE img_menu_bg;
extern Camera camera;

extern Audio bgm_menu;
extern Audio ui_confirm;

class mainmenu : public Screen
{
public:
    mainmenu()=default;
    ~mainmenu()=default;
    void enter(){
        bgm_menu.play(-1);
    }
    void draw(const Camera& camera) {
        camera.draw(&img_menu_bg);
    }
    void input(const SDL_Event& msg) {
        if(msg.type==SDL_KEYUP){
            ui_confirm.play();
            screenManager.switchTo(ScreenManager::ScreenType::Selector);
        }
        else if(msg.type==SDL_MOUSEBUTTONUP){
            ui_confirm.play();
            screenManager.switchTo(ScreenManager::ScreenType::Selector);
        }
    }
};