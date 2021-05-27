#ifndef MANAGER_H
#define MANAGER_H

#include <string.h>
#include "UIClasses.h"
#include "RiotClasses.h"
#include "UrlReader.h"
#include <iostream>
#include <vector>
#include <fstream>
using std::string;
using std::vector;

/**
 * Stores all the loaded matches
 */
class MatchStorage {
    public:
        // gets the pointer of an already loaded match given its match ID
        // if it can't find any it returns nullptr.
        Match* getMatch(string matchID) {
            for (Match* match : loadedMatches) {
                if (match->getMatchID() == matchID) return match;
            }
            return nullptr;
        }

        // loads a match from Riot's API given its matchID
        // matchID's are obtained from loadHistory()
        Match* loadMatch(string matchID) {
            Match* match = new Match(matchID);
            string URL = "https://americas.api.riotgames.com/lol/match/v5/matches/"+matchID;
            match->load(UrlReader::loadDoc(URL, false));
            loadedMatches.push_back(match);
            return match;
        }
        // loads the match history of a given player
        void loadHistory(UserPlayer * player);
    private:
        // stores all the loaded matches
        vector<Match*> loadedMatches;
};

class Manager {
    public:
        static Manager* instance; // allows static reference to Manager::instance
        Manager() { // CONSTRUCTOR
            instance = this;
            utility = new Utility();
            matchStorage = new MatchStorage();
            
        }

        // VARIABLES
        // all variables are public, no need for getters or setters.
        string currentSummoner; // stores the summoner name
        vector<Screen*> screens; // stores a dynamic list of created screens
        int currentScreenID;    // stores the id of current screen
        static int cooldown;    // blocks input while cooldown > 0
        WINDOW* inputWindow;    // the ncurses window
        Utility* utility;       // utility class for loading data
        MatchStorage* matchStorage; // stores all loaded matches
        Match* currentMatch;    // stores the current match loaded
        bool running = true;    // keeps the input and drawing loop alive
        static std::ofstream output; // debug logs


        // FUNCTIONS

        // sets the currentScreen and updates the window.
        void setScreen(int screenID) {
            currentScreenID = screenID;
            Selectable::selectedID = 0;
            cooldown = 5;
            clear();
            wclear(inputWindow);
            refreshScreen();
        }

        // loads match data into a list
        void loadMatch(MultiLineLabelList* list);
        // loads data about the match player into a list
        void loadMatchPlayer(MultiLineLabelList* list);

        // gets a screen from its index in the vector.
        Screen* getScreen(int id) { return screens.at(id); }
        // returns the current screen.
        Screen* getCurrentScreen() { return getScreen(currentScreenID); }

        // draws all the elements to the screen.
        void draw() {
            string str = "currentScreen: " + std::to_string(currentScreenID);
            //mvwaddstr(inputWindow, 1, 2, str.c_str());
            for (int i = 0; i < getCurrentScreen()->getElementCount(); i++) {
                Element* element = getCurrentScreen()->getElements()[i];
                (*element).draw(inputWindow);
            }
        }


        // gets the input from the user
        int input() {
            int choice = wgetch(inputWindow); // timesout after 100ms
            switch (choice) {
                case KEY_UP: // arrow keys
                case KEY_DOWN:
                    if (SelectableList* list = dynamic_cast<SelectableList*>(getCurrentScreen()->getSelected())) {
                        list->move(choice == KEY_UP);
                    }
                    break;
                case '\t': // tab
                    if (Selectable::selectedID < getCurrentScreen()->getSelectableCount()-1) {
                        Selectable::selectedID++;
                    } else {
                        Selectable::selectedID = 0;
                    }
                    return -9;
                case KEY_BACKSPACE: // backspace
                    return -10;
                case 353: // shift-tab
                    if (Selectable::selectedID > 0) {
                        Selectable::selectedID--;
                    } else {
                        Selectable::selectedID = getCurrentScreen()->getSelectableCount()-1;
                    }
                    return -8;
                case 10: // enter keys
                case KEY_ENTER:
                    return -7;
                default: break;
            }
            return choice;
        }

        // refrehes the window
        void refreshScreen() {
            box(inputWindow, 0, 0);
            refresh();
            wrefresh(inputWindow);
        }

        // load data from the DataDragon.
        bool loadUtility() {
            string spellsURL = "https://ddragon.leagueoflegends.com/cdn/11.10.1/data/en_US/summoner.json";
            string itemsURL = "https://ddragon.leagueoflegends.com/cdn/11.10.1/data/en_US/item.json";
            string queuesURL = "http://static.developer.riotgames.com/docs/lol/queues.json";
            utility->loadSpells(UrlReader::loadDoc(spellsURL, false)["data"].GetObject());
            utility->loadItems(UrlReader::loadDoc(itemsURL, false)["data"].GetObject());
            utility->loadQueues(UrlReader::loadDoc(queuesURL, false).GetArray());

            return true;
        }
    private:
        // cleans up memory.
        ~Manager() {
            delete[] inputWindow, utility, matchStorage, currentMatch;
        }
};

#endif