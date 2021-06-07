#include "Controller.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../UI/Buttons/BackButton.h"
#include "../UI/Labels/BoxedLabel.h"
#include "../UI/Labels/Label.h"
#include "../UI/Labels/MultiLineLabel.h"
#include "../UI/Lists/MultiLineLabelList.h"
using std::to_string;

// load match player from the list of players within a match
void Controller::loadMatchPlayer(MultiLineLabelList* list) {
    assert(list != nullptr);
    int selected = list->getSelectedLabel();
    // selected is the index of team
    // somehow need to differentiate between red and blue team.
    Match* match = this->currentMatch;  // get the match pointer
    MatchPlayer* player;                // player will be filled after
    if (list->getX() < 50) {
        // blue side
        player = match->getBlueTeam()->getPlayers().at(selected);
    } else {
        // red side
        player = match->getRedTeam()->getPlayers().at(selected);
    }

    // load match data for MatchUser
    Screen* screen = new Screen();
    int id = DisplayManager::instance->getScreens().size();
    string title = "Data for " + player->getChampName();
    screen->addElement(new BoxedLabel(title, 65 - title.length() / 2, 1, title.length() + 3, 2));
    string summoner = player->getSummonerName() + " - Level " + to_string(player->getLevel());
    screen->addElement(new Label(summoner, 65 - summoner.length() / 2, 4));
    LabelList* itemList = new LabelList(30, 13, 40, 7);
    if (!player->getItems().empty()) {
        for (int itemID : player->getItems()) {
            if (itemID > 0) {
                itemList->addLabel(utility->getItemName(itemID));
            }
        }
    }
    itemList->setTitle("Item List");
    screen->addElement(itemList);
    LabelList* spellList = new LabelList(30, 22, 40, 3);
    spellList->addLabel(utility->getSpellName(player->getSpellD()));
    spellList->addLabel(utility->getSpellName(player->getSpellF()));
    spellList->setTitle("Summoner Spells");
    screen->addElement(spellList);

    LabelList* kdaList = new LabelList(72, 13, 30, 4);

    float kda = (float)(player->getKills() + player->getAssists()) / (float)player->getDeaths();
    kda = roundf(kda * 100.0) / 100.0;
    std::ostringstream oss;
    oss << std::setprecision(2) << std::noshowpoint << kda;
    std::string str = oss.str();

    kdaList->setTitle("KDA: " + str);
    kdaList->addLabel("Kills: " + to_string(player->getKills()));
    kdaList->addLabel("Deaths: " + to_string(player->getDeaths()));
    kdaList->addLabel("Assists: " + to_string(player->getAssists()));
    screen->addElement(kdaList);

    LabelList* information = new LabelList(72, 19, 30, 10);
    information->addLabel("CS: " + to_string(player->getCreepScore()));

    // calculate cs per minute

    float csMin = (float)player->getCreepScore() / (Utility::getMinutes(match->getDuration()));
    csMin = roundf(csMin * 100.0) / 100.0;
    std::ostringstream oss2;
    oss2 << std::setprecision(2) << std::noshowpoint << csMin;
    str = oss2.str();
    information->addLabel("CS/min: " + str);
    information->addLabel("Gold Earnt: " + to_string(player->getGoldEarned()));
    information->addLabel("Vision Score: " + to_string(player->getVisionScore()));
    information->addLabel("Control Wards: " + to_string(player->getPinkWardsPlaced()));
    information->addLabel("Champion Level: " + to_string(player->getChampLevel()));
    if (player->getLane() != "NONE") information->addLabel("Assigned Lane: " + (player->getLane()));
    information->setHeight(information->getLabels().size() + 1);
    information->setTitle("Information");
    screen->addElement(information);

    screen->addElement(new BackButton("  Back  ", 62, 38, 10, 2, DisplayManager::instance->getCurrentScreenID()));

    DisplayManager::instance->addScreen(screen);
    DisplayManager::instance->setScreen(id);
}

string getWinString(bool win) {
    return win ? "Victory" : "Loss";
}

void Controller::loadMatch(MultiLineLabelList* list) {
    assert(list != nullptr);
    int selected = list->getSelectedLabel();
    assert(selected < 1000000);
    assert(selected < list->getLabels().size());
    MultiLineLabel* label = list->getLabels().at(selected);
    assert(label != nullptr);
    string matchID = label->getHiddenValue();
    Match* match = matchStorage->getMatch(matchID);
    currentMatch = match;

    Screen* screen = new Screen();
    int id = DisplayManager::instance->getScreens().size();
    string title = "  Match Report  ";
    screen->addElement(new BoxedLabel(title, 65 - title.length() / 2, 1, 20, 2));

    // blue side
    Label* blueLabel = new Label("Blue Side - " + getWinString(match->getBlueTeam()->didWin()), 3, 4);
    Label* redLabel = new Label("Red Side  - " + getWinString(match->getRedTeam()->didWin()), 70, 4);
    screen->addElement(blueLabel);
    screen->addElement(redLabel);
    screen->addElement(new Label("vs", 64, 20));

    MultiLineLabelList* blueList = new MultiLineLabelList(3, 6, 60, 25);
    MultiLineLabelList* redList = new MultiLineLabelList(70, 6, 60, 25);

    for (int i = 0; i < 5; i++) {
        MultiLineLabel* blueMember = new MultiLineLabel(match->getBlueTeam()->getPlayers().at(i)->getDisplayCard());
        blueList->addLabel(blueMember);
        MultiLineLabel* redMember = new MultiLineLabel(match->getRedTeam()->getPlayers().at(i)->getDisplayCard());
        redList->addLabel(redMember);
    }

    LabelList* blueTeamList = new LabelList(12, 32, 40, 4);
    blueTeamList->addLabel("Dragons: " + to_string(match->getBlueTeam()->getDragonKills()));
    blueTeamList->addLabel("Rift Heralds: " + to_string(match->getBlueTeam()->getRiftHeralds()));
    blueTeamList->addLabel("Barons: " + to_string(match->getBlueTeam()->getBarons()));

    LabelList* redTeamList = new LabelList(79, 32, 40, 4);
    redTeamList->addLabel("Dragons: " + to_string(match->getRedTeam()->getDragonKills()));
    redTeamList->addLabel("Rift Heralds: " + to_string(match->getRedTeam()->getRiftHeralds()));
    redTeamList->addLabel("Barons: " + to_string(match->getRedTeam()->getBarons()));

    string goldDifferenceTitle = "Gold Difference";
    screen->addElement(new Label(goldDifferenceTitle, 65 - goldDifferenceTitle.length() / 2, 32));
    string goldDifference = match->goldGap() > 0 ? "+" + to_string(match->goldGap()) : to_string(match->goldGap());
    screen->addElement(new Label(goldDifference, 65 - goldDifference.length() / 2, 34));
    screen->addElement(blueTeamList);
    screen->addElement(redTeamList);

    screen->addElement(blueList);
    screen->addElement(redList);

    screen->addElement(new BackButton("  Back  ", 62, 38, 10, 2, 2));

    DisplayManager::instance->addScreen(screen);
    DisplayManager::instance->setScreen(id);
}