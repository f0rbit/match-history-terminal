#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H
#include <ncurses.h>

#include "DisplayManager.h"
class InputManager {
   public:
    // gets the input from the user
    int input();
    void setWindow(WINDOW* window) { this->inputWindow = window; };
    Screen* getCurrentScreen() { return DisplayManager::instance->getCurrentScreen(); };

    static InputManager* instance;

   protected:
    WINDOW* inputWindow;
};

#endif