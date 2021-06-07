#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <ncurses.h>
#include <string.h>

#include <fstream>
#include <iostream>
#include <vector>

#include "../Riot/Utility.h"
#include "../UI/Lists/MultiLineLabelList.h"
#include "../UI/Screen.h"
#include "DisplayManager.h"
#include "InputManager.h"
#include "MatchStorage.h"
using std::string;
using std::vector;

class Controller {
   public:
    static Controller* instance;  // allows static reference to Controller::instance
    Controller() {                // CONSTRUCTOR
        instance = this;
        utility = new Utility();
        matchStorage = new MatchStorage();
    }

    MatchStorage* matchStorage;
    // VARIABLES
    // all variables are public, no need for getters or setters.
    string currentSummoner;       // stores the summoner name
    WINDOW* inputWindow;          // the ncurses window
    Utility* utility;             // utility class for loading data
    Match* currentMatch;          // stores the current match loaded
    bool running = true;          // keeps the input and drawing loop alive
    static std::ofstream output;  // debug logs

    // FUNCTIONS

    void setInputWindow(WINDOW* window) {
        InputManager::instance->setWindow(window);
        DisplayManager::instance->setWindow(window);
        inputWindow = window;
    }
    // loads match data into a list
    void loadMatch(MultiLineLabelList* list);
    // loads data about the match player into a list
    void loadMatchPlayer(MultiLineLabelList* list);

   private:
    // cleans up memory.
    ~Controller() {
        delete[] inputWindow, utility, currentMatch;
    }
};

#endif