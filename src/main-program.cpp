#include "include/convert.h"
#include "UIClasses.h"
#include "Manager.h"
#include <vector>
#include <fstream>
using namespace std;

Manager* Manager::instance = new Manager(); // creates an instance of the manager
std::ofstream Manager::output ("output.txt"); // output logs

// returns the screen for summoner name entry
Screen* getSummonerScreen() {
    Screen* summonerScreen = new Screen(); // makes a new screen
    InputBox* box = new InputBox(49,15,30,2); // input box for summoner name entry
    EnterButton* enterButton = new EnterButton("  Enter  ", 50, 23, 10, 2); // button to click after entering summoner name
    enterButton->setBox(box); // sets the box to get input from when pressing enter
    QuitButton* quitButton = new QuitButton("  Quit  ", 69,23,9,2); // button for exiting program

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
    Screen* matchHistoryScreen = new Screen(); // makes new screen
    Label* label = new Label("Match History", 57, 0); // title
    matchHistoryScreen->addElement(label);
    matchHistoryScreen->addElement(new MultiLineLabelList(3,2,90,39)); // the box that stores all the matches
    matchHistoryScreen->addElement(new BackButton("  Back  ",105,39,10,2,1)); // back button to go back to summoner name entry
    matchHistoryScreen->addElement(new LabelList(94,2,36,36)); // list for information about recent games
    return matchHistoryScreen;
}

int main() {

    // ask the user to resize their terminal to appropiate size.
    cout << "Please resize your terminal to 132x43 and then press enter." << endl;
    cin.get(); // pause until user presses enter

    // start the ncurses window
    initscr();
    noecho();
    cbreak();

    // set size
    int yMax, xMax;
    getmaxyx(stdscr, yMax, xMax);

    // draw box
    WINDOW* inputwin = newwin(0, xMax-1, yMax-43, 1);
    box(inputwin, 0, 0);
    refresh();
    curs_set(0);

    
    Manager* manager = Manager::instance; // so that we don't have to call Manager::instance-> all the time

    // some more ncurses initialisation
    wrefresh(inputwin); 
    keypad(inputwin, true);
    // set the input window
    manager->inputWindow = inputwin;

    int choice; // the character pressed
    // show loading screen
    Screen* loadingScreen = new Screen();
    loadingScreen->addElement(new Label("Loading...",59,15));
    loadingScreen->addElement(new Label("Please wait.", 58, 17));

    // display loading screen while we wait to load data from DataDragon
    manager->screens.push_back(loadingScreen);
    manager->draw();
    // push the drawing buffer to the window
    wrefresh(inputwin);
    wtimeout(inputwin, 100);
    refresh();
    
    // load data from Riot's DataDragon
    while (!manager->loadUtility()) {}

    Label* label = (Label*) loadingScreen->getElements()[0];
    // sets the text for use after entering name
    label->setText("Loading 0/13 matches...");
    label->setX(52);

    manager->screens.push_back(getSummonerScreen());
    manager->screens.push_back(getMatchHistoryScreen());

    manager->setScreen(1); // sets the current screen to the Summoner Name Entry
    manager->draw();

    wrefresh(inputwin);
    wtimeout(inputwin, 100);
    refresh();

    // the loop that takes input and draws everything to the screen
    while (Manager::instance->running) { // terminates if Manager::instance->running is false. (only gets set to false in the QuitButton)
        // SCREEN LOOP
        choice = manager->input(); // get the input from the user
        char c = choice; // cast the choice to a character
        if (Manager::cooldown > 0) {
            Manager::cooldown--; // cooldown determines wether elements are interactable or not.
            continue;
        }
        // loop through all the elements and do special stuff with specific element types
        for (int i = 0; i < manager->getCurrentScreen()->getElementCount(); i++) {
            if (DisappearingLabel* label = dynamic_cast<DisappearingLabel*>(manager->getCurrentScreen()->getElements()[i])) {
                label->tick(); // make the label disappear after some time.
            }
            if(Inputable* input = dynamic_cast<Inputable*>(manager->getCurrentScreen()->getElements()[i])) {
                if (input->isSelected(input)) {
                    input->inputCharacter(choice, c); // add character to InputBox
                    break;
                }
            }
            if (Button* button = dynamic_cast<Button*>(manager->getCurrentScreen()->getElements()[i])) {
                if (choice == -7 && button->isSelected(button) && Manager::cooldown <= 0) {
                    button->execute();  // if enter is pressed execute the button's action
                    break;
                }
            }
            if (MultiLineLabelList* list = dynamic_cast<MultiLineLabelList*>(manager->getCurrentScreen()->getElements()[i])) {
                if (choice == -7 && list->isSelected(list) && Manager::cooldown <= 0) {
                    if (manager->currentScreenID == 2) {
                        manager->loadMatch(list);   // load match based off currently selected index
                        break;
                    } else {
                        // load the screen for the match player
                        manager->loadMatchPlayer(list);
                        break;
                    }
                }
            }        
        }
        // draw all the elements to the sreen
        manager->draw();
        refresh(); // send the drawing buffer to the window
    }

    // reset the terminal to its original state.
    curs_set(1);
    endwin();
    return 0;

}