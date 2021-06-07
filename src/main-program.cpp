#include <ncurses.h>

#include <vector>

#include "classes/Managers/Controller.h"
#include "classes/UI/Buttons/BackButton.h"
#include "classes/UI/Buttons/EnterButton.h"
#include "classes/UI/Buttons/QuitButton.h"
#include "classes/UI/InputBox.h"
#include "classes/UI/Labels/DisappearingLabel.h"
#include "classes/UI/Labels/Label.h"
using namespace std;

Controller* Controller::instance = new Controller();  // creates an instance of the manager

// returns the screen for summoner name entry
Screen* getSummonerScreen() {
    Screen* summonerScreen = new Screen();                                   // makes a new screen
    InputBox* box = new InputBox(49, 15, 30, 2);                             // input box for summoner name entry
    EnterButton* enterButton = new EnterButton("  Enter  ", 50, 23, 10, 2);  // button to click after entering summoner name
    enterButton->setBox(box);                                                // sets the box to get input from when pressing enter
    QuitButton* quitButton = new QuitButton("  Quit  ", 69, 23, 9, 2);       // button for exiting program

    // add elements to screen
    summonerScreen->addElement(box);
    summonerScreen->addElement(new Label("Enter Summoner Name", 54, 19));
    summonerScreen->addElement(enterButton);
    summonerScreen->addElement(quitButton);

    // return screen pointer.
    return summonerScreen;
}

// returns the base screen for match history
Screen* getMatchHistoryScreen() {
    Screen* matchHistoryScreen = new Screen();         // makes new screen
    Label* label = new Label("Match History", 57, 0);  // title
    matchHistoryScreen->addElement(label);
    matchHistoryScreen->addElement(new MultiLineLabelList(3, 2, 90, 39));           // the box that stores all the matches
    matchHistoryScreen->addElement(new BackButton("  Back  ", 105, 39, 10, 2, 1));  // back button to go back to summoner name entry
    matchHistoryScreen->addElement(new LabelList(94, 2, 36, 36));                   // list for information about recent games
    return matchHistoryScreen;
}

int main() {
    // ask the user to resize their terminal to appropiate size.
    cout << "Please resize your terminal to 132x43 and then press enter." << endl;
    cin.get();  // pause until user presses enter

    // start the ncurses window
    initscr();
    noecho();
    cbreak();

    // set size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // draw box
    WINDOW* inputwin = newwin(43, 132, 0, 0);
    box(inputwin, 0, 0);
    refresh();
    curs_set(0);

    Controller* controller = Controller::instance;  // so that we don't have to call Coiontroller::instance-> all the time
    // some more ncurses initialisation
    wrefresh(inputwin);
    keypad(inputwin, true);
    // set the input window
    controller->setInputWindow(inputwin);

    int choice;  // the character pressed
    // show loading screen
    Screen* loadingScreen = new Screen();
    loadingScreen->addElement(new Label("Loading...", 59, 15));
    loadingScreen->addElement(new Label("Please wait.", 58, 17));
    // display loading screen while we wait to load data from DataDragon
    DisplayManager::instance->addScreen(loadingScreen);
    DisplayManager::instance->draw();
    // push the drawing buffer to the window
    DisplayManager::instance->refreshScreen();
    wtimeout(inputwin, 100);

    // load data from Riot's DataDragon
    while (!controller->utility->loadData()) {
    }

    Label* label = (Label*)loadingScreen->getElements()[0];
    // sets the text for use after entering name
    label->setText("Loading 0/13 matches...");
    label->setX(52);

    DisplayManager::instance->addScreen(getSummonerScreen());
    DisplayManager::instance->addScreen(getMatchHistoryScreen());

    DisplayManager::instance->setScreen(1);  // sets the current screen to the Summoner Name Entry
    DisplayManager::instance->draw();
    DisplayManager::instance->refreshScreen();

    // the loop that takes input and draws everything to the screen
    while (controller->running) {  // terminates if Manager::instance->running is false. (only gets set to false in the QuitButton)
        // SCREEN LOOP
        choice = InputManager::instance->input();  // get the input from the user
        char c = choice;                           // cast the choice to a character

        if (DisplayManager::instance->cooldown > 0) {
            DisplayManager::instance->cooldown--;  // cooldown determines wether elements are interactable or not.
            continue;
        }
        // loop through all the elements and do special stuff with specific element types
        for (int i = 0; i < DisplayManager::instance->getCurrentScreen()->getElementCount(); i++) {
            if (DisappearingLabel* label = dynamic_cast<DisappearingLabel*>(DisplayManager::instance->getCurrentScreen()->getElements()[i])) {
                label->tick();  // make the label disappear after some time.
            }
            if (Inputable* input = dynamic_cast<Inputable*>(DisplayManager::instance->getCurrentScreen()->getElements()[i])) {
                if (input->isSelected(input)) {
                    input->inputCharacter(choice, c);  // add character to InputBox
                    break;
                }
            }
            if (Button* button = dynamic_cast<Button*>(DisplayManager::instance->getCurrentScreen()->getElements()[i])) {
                if (choice == -7 && button->isSelected(button) && DisplayManager::instance->cooldown <= 0) {
                    button->execute();  // if enter is pressed execute the button's action
                    break;
                }
            }
            if (MultiLineLabelList* list = dynamic_cast<MultiLineLabelList*>(DisplayManager::instance->getCurrentScreen()->getElements()[i])) {
                if (choice == -7 && list->isSelected(list) && DisplayManager::instance->cooldown <= 0) {
                    if (DisplayManager::instance->getCurrentScreenID() == 2) {
                        controller->loadMatch(list);  // load match based off currently selected index
                        break;
                    } else {
                        // load the screen for the match player
                        controller->loadMatchPlayer(list);
                        break;
                    }
                }
            }
        }
        // draw all the elements to the sreen
        DisplayManager::instance->draw();
        DisplayManager::instance->refreshScreen();
    }

    // reset the terminal to its original state.
    curs_set(1);
    endwin();
    return 0;
}