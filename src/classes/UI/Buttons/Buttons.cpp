#include "../../Managers/Controller.h"
#include "../Labels/DisappearingLabel.h"
#include "BackButton.h"
#include "Button.h"
#include "EnterButton.h"
#include "QuitButton.h"

void Button::draw() {
    // draws the button
    // will be highlighted with white background if it's selected.
    if (isSelected(this)) attron(A_STANDOUT);
    Boxed::drawRectangle(getX(), getY(), getWidth(), getHeight());
    mvaddstr(getY() + 1, getX() + 1, getText().c_str());
    attroff(A_STANDOUT);
}

void QuitButton::execute() {
    Controller::instance->running = false;
}

void BackButton::execute() {
    // go to the previous screen.
    DisplayManager::instance->setScreen(screenID);
    DisplayManager::instance->draw();
    DisplayManager::instance->refreshScreen();
}

void EnterButton::execute() {
    // enter button for summoner name entry
    string summonerName = getBox()->getInput();  // gets the summoner name from InputBox
    getBox()->clearInput();                      // clears the input box

    UserPlayer* player = new UserPlayer();                    // makes a new User Player
    player->setSummonerName(summonerName);                    // sets the name
    if (Controller::instance->utility->loadPlayer(player)) {  // load data about player from riot api
        // execute if player is found
        // goto loading screen
        DisplayManager::instance->setScreen(0);
        DisplayManager::instance->draw();
        DisplayManager::instance->refreshScreen();
        // load match history
        Controller::instance->matchStorage->loadHistory(player);
        // goto match history screen
        DisplayManager::instance->setScreen(2);
    } else {
        // error <player not found!>
        string str = player->getSummonerName() + " is not a recognized summoner!";
        DisplayManager::instance->getCurrentScreen()->addElement(new DisappearingLabel(str, 65 - str.length() / 2, 27));  // DisappearingLabel goes away after some time.
    }
}