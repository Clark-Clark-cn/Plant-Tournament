#pragma once
#include "screen.h"

extern Screen* Menu;
extern Screen* Game;
extern Screen* Selector;

extern bool is_debug;

class ScreenManager
{
public:
    enum class ScreenType {
        MainMenu,
        Game,
        Selector
    };
    ScreenManager()=default;
    ~ScreenManager()=default;
    void setCurrentState(Screen* newScreen)
    {
        currentScreen = newScreen;
        currentScreen->enter();
    }
    void switchTo(ScreenType newScreenType)
    {
        currentScreen->exit();
        switch (newScreenType)
        {
        case ScreenType::MainMenu:
            currentScreen = Menu;
            break;
        case ScreenType::Game:
            currentScreen = Game;
            break;
        case ScreenType::Selector:
            currentScreen = Selector;
            break;
        }
        currentScreen->enter();
    }
    void input(const ExMessage& msg)
    {
        if (currentScreen)
        {
            currentScreen->input(msg);
        }
        if(msg.message==WM_KEYUP&&msg.vkcode==VK_ESCAPE)
        {
            exit(0);
        }
        else if (msg.message==WM_KEYUP&&msg.vkcode==VK_F1)
        {
            is_debug = !is_debug;
        }
    }
    void update(int delta)
    {
        if (currentScreen)
        {
            currentScreen->update(delta);
        }
    }
    void draw(const Camera& camera)
    {
        if (currentScreen)
        {
            currentScreen->draw(camera);
        }
    }
private:
    Screen* currentScreen=nullptr;
};
