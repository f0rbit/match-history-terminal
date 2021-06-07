#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include <ncurses.h>

#include <string>
#include <vector>

#include "../UI/Element.h"
#include "../UI/Screen.h"
#include "../UI/Selectable.h"
class DisplayManager {
   public:
    // sets the currentScreen and updates the window.
    void setScreen(int screenID) {
        currentScreenID = screenID;
        Selectable::selectedID = 0;
        cooldown = 5;
        clear();
        wclear(inputWindow);
        refreshScreen();
    }
    // gets a screen from its index in the vector.
    Screen* getScreen(int id) { return screens.at(id); }
    // returns the current screen.
    Screen* getCurrentScreen() { return getScreen(currentScreenID); }

    std::vector<Screen*> getScreens() { return this->screens; };
    void addScreen(Screen* screen) {
        this->screens.emplace_back(screen);
    }
    int getCurrentScreenID() { return this->currentScreenID; };

    // draws all the elements to the screen.
    void draw() {
        std::string str = "currentScreen: " + std::to_string(currentScreenID);
        //mvwaddstr(inputWindow, 1, 2, str.c_str());
        for (int i = 0; i < getCurrentScreen()->getElementCount(); i++) {
            Element* element = getCurrentScreen()->getElements()[i];
            (*element).draw();
        }
    }

    void setWindow(WINDOW* window) { this->inputWindow = window; };

    // refrehes the window
    void refreshScreen() {
        box(inputWindow, 0, 0);
        wrefresh(inputWindow);
        refresh();
    }
    int cooldown;

    static DisplayManager* instance;

   private:
    WINDOW* inputWindow;
    int currentScreenID;
    std::vector<Screen*> screens = {};
};

#endif