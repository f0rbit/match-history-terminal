#include "UIClasses.h"
#include "RiotClasses.h"
#include "UrlReader.h"
#include "Manager.h"
#include <math.h>
#include "../include/ncurses.h"
#include <sstream>
#include <iomanip>
#include <fstream>

using std::to_string;

int Element::currentID = 0;
int Selectable::selectedID = 0;
int Screen::screenCount = 0;
int Manager::cooldown = 10;

void drawRectangle(int x, int y, int width, int height) {
    // fills the rectangle with spaces
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            mvaddch(y+j,x+i,' ');
        }
    }
    // draws box around
    mvhline(y, x, 0, x+width-x);
    mvhline( y+height, x, 0, x+width-x);
    mvvline( y, x, 0, y+height-y);
    mvvline( y, x+width, 0, y+height-y);
    mvaddch( y, x, ACS_ULCORNER);
    mvaddch( y+height, x, ACS_LLCORNER);
    mvaddch( y, x+width, ACS_URCORNER);
    mvaddch( y+height, x+width, ACS_LRCORNER);
}

void Boxed::drawBox(WINDOW* window) {
    // draws a box
    drawRectangle(getX(), getY(), getWidth(), getHeight());
}

void Label::draw(WINDOW* window) {
    // draws the label
    mvwaddstr(window, getY()+1, getX()+1, getText().c_str());
}

void MultiLineLabel::draw(WINDOW* window, int x, int y) {
    // draws a list of strings
    for (int i = 0; i < getStrings().size(); i++) {
        mvaddstr(y+i,x,getStrings().at(i).c_str());
    }
}

void BoxedLabel::draw(WINDOW* window) {
    // draws a label with a box around it
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    mvwaddstr(window, getY()+1, getX()+1, getText().c_str());
}

void Button::draw(WINDOW* window) {
    // draws the button
    // will be highlighted with white background if it's selected.
    if (isSelected(this)) attron(A_STANDOUT);
    drawRectangle(getX(),getY(),getWidth(), getHeight());
    mvaddstr(getY()+1, getX()+1, getText().c_str());
    attroff(A_STANDOUT);
}

void InputBox::draw(WINDOW* window) {
    // draws an input box
    // will have white highlight if selelected
    if (isSelected(this)) {
        attron(A_STANDOUT);
    }
    drawRectangle( getX(), getY(), getWidth(), getHeight());
    mvaddch(getY()+1, getX()+1, ('>'));
    mvaddstr(getY()+1, getX()+3, getInput().c_str());
    attroff(A_STANDOUT);
}

void LabelList::draw(WINDOW* window) {
    // draws a list of strings.
    mvwaddstr(window,getY()-1,getX()+1,getTitle().c_str());
    drawRectangle( getX(), getY(), getWidth(), getHeight());
    for (int i = 0; i < getLabels().size(); i++) {
        mvaddstr(getY()+i+1,getX()+1,getLabels().at(i).c_str());
    }
}

void MultiLineLabelList::draw(WINDOW* window) {
    // draws a list of lists of strings with lines inbetween.
    mvwaddstr(window,getY()-1,getX()+1,getTitle().c_str());
    drawRectangle( getX(), getY(), getWidth(), getHeight());
    int currentY = getY()+1; // keep track of y position
    for (int i = 0; i < getLabels().size(); i++) {
        if (i > 0) {
            mvhline(currentY++, x+1, 0, getWidth()-1);
        }
        if (isSelected(this) && selectedLabel == i) {
            attron(A_STANDOUT);
        }
        getLabels().at(i)->draw(window, getX()+1,currentY);
        currentY += getLabels().at(i)->getStrings().size();
        attroff(A_STANDOUT);
    }
}

void MultiLineLabelList::move(bool up) {
    if (up) {
        if (selectedLabel > 0) selectedLabel--;
    } else {
        if (selectedLabel < labels.size()-1) selectedLabel++;
    }
}

void SelectableList::draw(WINDOW* window) {
    mvwaddstr(window,getY()-1,getX()+1,getTitle().c_str());
    drawRectangle( getX(), getY(), getWidth(), getHeight());
    for (int i = 0; i < getLabels().size(); i++) {
        if (isSelected(this) && selectedLabel == i) {
            attron(A_STANDOUT);
        }
        mvaddstr(getY()+i+1,getX()+1,getLabels().at(i).c_str());
        attroff(A_STANDOUT);
    }

}

void SelectableList::move(bool up) {
    if (up) {
        if (selectedLabel > 0) selectedLabel--;
    } else {
        if (selectedLabel < labels.size()-1) selectedLabel++;
    }
}

void ScrollableList::draw(WINDOW* window) {
    mvwaddstr(window,getY()-1,getX()+1,getTitle().c_str());
    drawRectangle( getX(), getY(), getWidth(), getHeight());
    for (int i = yOffset; i < getHeight()-1+yOffset; i++) {
        if (isSelected(this) && selectedLabel == i) {
            attron(A_STANDOUT);
        }
        mvaddstr(getY()+i+1-yOffset,getX()+1,getLabels().at(i).c_str());
        attroff(A_STANDOUT);
    }

    // draw arrows if more elements are present off screen
    if (yOffset < getHeight()-3) {
        mvaddch(getY()+getHeight()-1,getX()+getWidth()-1,'v');
    }
    if (yOffset > 0) {
        mvaddch(getY()+1,getX()+getWidth()-1, '^');
    }
}


void ScrollableList::move(bool up) {
    if (up) {
        if (selectedLabel > 0) {
            selectedLabel--;

            if (selectedLabel < yOffset) {
                yOffset--;
            }
        }
    } else {
        if (selectedLabel < labels.size()-1) {
            selectedLabel++;

            if (selectedLabel > getHeight()-2) {
                yOffset++;
            }
        }
    }
}


void InputBox::inputCharacter(int choice, char c) {
    string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 "; // all the characters that are accepted by InputBox
    if (choice == -9 || choice == -8 || choice == -7 || choice == -1) return; // if it's enter, nothing, or an arrow key.
    if (isSelected(this)) { // only works if this InputBox is selected.
        if (choice == -10) { // backspace
            delCh();    // delete the last character
        } else {
            if (getInput().length() < width-3) {
                if (acceptedCharacters.find(c) == string::npos) return; // checks to see if there is an instance of (char) c inside (string) acceptedCharacters, returns out if not.
                addCh(c);   // add the character to the inputbox.text
            }
        }
    }
}

void EnterButton::execute() {
    // enter button for summoner name entry
    string summonerName = getBox()->getInput(); // gets the summoner name from InputBox
    getBox()->clearInput(); // clears the input box

    Manager::output << "Looking for player: " << summonerName << endl;
    Manager::output.flush();   
    UserPlayer * player = new UserPlayer(); // makes a new User Player
    player->setSummonerName(summonerName); // sets the name
    if (Manager::instance->utility->loadPlayer(player)) { // load data about player from riot api
        // execute if player is found
        // goto loading screen
        Manager::instance->setScreen(0);
        Manager::instance->draw();
        Manager::instance->refreshScreen();
        // load match history
        Manager::instance->matchStorage->loadHistory(player);
        // goto match history screen
        Manager::instance->setScreen(2);
    } else {
        // error <player not found!>
        string str = player->getSummonerName() +" is not a recognized summoner!";
        Manager::instance->getCurrentScreen()->addElement(new DisappearingLabel(str, 65-str.length()/2, 27)); // DisappearingLabel goes away after some time.
    }
}

void DisappearingLabel::draw(WINDOW* window) {
    if (cooldown > 0) {
        mvaddstr(getY()+1, getX()+1, getText().c_str());
    } else {
        mvhline(getY()+1,getX()+1,' ',getText().length());
    }
}

void BackButton::execute() {
    // go to the previous screen.
    Manager::instance->setScreen(screenID);
    Manager::instance->draw();
    Manager::instance->refreshScreen();
}
Screen::Screen() {
    // screen constructor
    elements = (new Element*[0]);
    elementCount = 0;
    selectableInstances = (new Selectable*[0]);
    selectableCount = 0;
}


// gets the amount of minutes from millisecond duration
int getMinutes(long milliseconds) {
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    int minutes = seconds / 60;
    seconds %= 60;
    return minutes;

}

// load match player from the list of players within a match
void Manager::loadMatchPlayer(MultiLineLabelList* list) {
    assert(list != nullptr);
    int selected = list->getSelectedLabel();
    // selected is the index of team
    // somehow need to differentiate between red and blue team.
    Match* match = Manager::instance->currentMatch; // get the match pointer
    MatchPlayer* player; // player will be filled after
    if (list->getX() < 50) {
        // blue side
        player = match->getBlueTeam()->getPlayers().at(selected);
    } else {
        // red side
        player = match->getRedTeam()->getPlayers().at(selected);
    }

    // load match data for MatchUser
    Screen* screen = new Screen();
    int id = Manager::instance->screens.size();
    string title = "Data for "+player->getChampName();
    screen->addElement(new BoxedLabel(title,65-title.length()/2,1,title.length()+3,2));
    string summoner = player->getSummonerName()+" - Level "+to_string(player->getLevel());
    screen->addElement(new Label(summoner, 65-summoner.length()/2, 4));
    LabelList* itemList = new LabelList(30,13,40,7);
    if (!player->getItems().empty()) {
        for (int itemID : player->getItems()) {
            if (itemID <= 0) continue;
            itemList->addLabel(Manager::instance->utility->getItemName(itemID));
        }
    }
    itemList->setTitle("Item List");
    screen->addElement(itemList);
    LabelList* spellList = new LabelList(30,22,40,3);
    spellList->addLabel(Manager::instance->utility->getSpellName(player->getSpellD()));
    spellList->addLabel(Manager::instance->utility->getSpellName(player->getSpellF()));
    spellList->setTitle("Summoner Spells");
    screen->addElement(spellList);

    LabelList* kdaList = new LabelList(72,13,30,4);

    float kda = (float) (player->getKills()+player->getAssists()) / (float) player->getDeaths();
    kda = roundf(kda*100.0) / 100.0;
    std::ostringstream oss;
    oss << std::setprecision(2) << std::noshowpoint << kda;
    std::string str = oss.str();

    kdaList->setTitle("KDA: "+str);
    kdaList->addLabel("Kills: "+to_string(player->getKills()));
    kdaList->addLabel("Deaths: "+to_string(player->getDeaths()));
    kdaList->addLabel("Assists: "+to_string(player->getAssists()));
    screen->addElement(kdaList);

    LabelList* information = new LabelList(72,19,30,10);
    information->addLabel("CS: " +to_string(player->getCreepScore()));

    // calculate cs per minute

    float csMin = (float) player->getCreepScore() / (getMinutes(match->getDuration()));
    csMin = roundf(csMin*100.0) / 100.0;
    std::ostringstream oss2;
    oss2 << std::setprecision(2) << std::noshowpoint << csMin;
    str = oss2.str();
    information->addLabel("CS/min: "+str);
    information->addLabel("Gold Earnt: "+ to_string(player->getGoldEarned()));
    information->addLabel("Vision Score: "+to_string(player->getVisionScore()));
    information->addLabel("Control Wards: " + to_string(player->getPinkWardsPlaced()));
    information->addLabel("Champion Level: "+ to_string(player->getChampLevel()));
    if (player->getLane() != "NONE") information->addLabel("Assigned Lane: "+(player->getLane()));
    information->setHeight(information->getLabels().size()+1);
    information->setTitle("Information");
    screen->addElement(information);

    screen->addElement(new BackButton("  Back  ", 62, 38, 10, 2, Manager::instance->currentScreenID));

    Manager::instance->screens.push_back(screen);
    Manager::instance->setScreen(id);
}
string getWinString(bool win) {
    return win ? "Victory" : "Loss";
}
void Manager::loadMatch(MultiLineLabelList* list) {
    assert(list != nullptr);
    int selected = list->getSelectedLabel();
    assert (selected < 1000000);
    assert(selected < list->getLabels().size());
    MultiLineLabel* label = list->getLabels().at(selected);
    assert(label != nullptr);
    string matchID = label->getHiddenValue();
    Match* match = Manager::instance->matchStorage->getMatch(matchID);
    Manager::instance->currentMatch = match;

    Screen* screen = new Screen();
    int id = Manager::instance->screens.size();
    string title = "  Match Report  ";
    screen->addElement(new BoxedLabel(title, 65-title.length()/2,1,20,2));

    // blue side
    Label* blueLabel = new Label("Blue Side - "+getWinString(match->getBlueTeam()->didWin()),3,4);
    Label* redLabel = new Label("Red Side  - "+getWinString(match->getRedTeam()->didWin()), 70,4);
    screen->addElement(blueLabel);
    screen->addElement(redLabel);
    screen->addElement(new Label("vs", 64, 20));

    MultiLineLabelList * blueList = new MultiLineLabelList(3,6,60,25);
    MultiLineLabelList * redList = new MultiLineLabelList(70,6,60,25);

    for (int i = 0; i < 5; i++) {
        MultiLineLabel * blueMember = new MultiLineLabel(match->getBlueTeam()->getPlayers().at(i)->getDisplayCard());
        blueList->addLabel(blueMember);
        MultiLineLabel * redMember = new MultiLineLabel(match->getRedTeam()->getPlayers().at(i)->getDisplayCard());
        redList->addLabel(redMember);
    }

    LabelList * blueTeamList = new LabelList(12,32, 40, 4);
    blueTeamList->addLabel("Dragons: "+to_string(match->getBlueTeam()->getDragonKills()));
    blueTeamList->addLabel("Rift Heralds: "+to_string(match->getBlueTeam()->getRiftHeralds()));
    blueTeamList->addLabel("Barons: "+to_string(match->getBlueTeam()->getBarons()));

    LabelList * redTeamList = new LabelList(79,32, 40, 4);
    redTeamList->addLabel("Dragons: "+to_string(match->getRedTeam()->getDragonKills()));
    redTeamList->addLabel("Rift Heralds: "+to_string(match->getRedTeam()->getRiftHeralds()));
    redTeamList->addLabel("Barons: "+to_string(match->getRedTeam()->getBarons()));

    string goldDifferenceTitle = "Gold Difference";
    screen->addElement(new Label(goldDifferenceTitle, 65-goldDifferenceTitle.length()/2,34));
    string goldDifference = match->goldGap() > 0 ? "+"+to_string(match->goldGap()) : to_string(match->goldGap());
    screen->addElement(new Label(goldDifference, 65-goldDifference.length()/2, 35));
    screen->addElement(blueTeamList);
    screen->addElement(redTeamList);


    screen->addElement(blueList);
    screen->addElement(redList);

    screen->addElement(new BackButton("  Back  ", 62,38,10,2,2));


    Manager::instance->screens.push_back(screen);
    Manager::instance->setScreen(id);

}

void MatchStorage::loadHistory(UserPlayer* player) {
    string URL = "https://americas.api.riotgames.com/lol/match/v5/matches/by-puuid/"+player->getPUUID()+"/ids?start=0&count=13&api_key="+UrlReader::API_KEY;
    Document doc = UrlReader::loadDoc(URL, true);
    GenericArray<false, Value> array = doc.GetArray();
    vector<string> last10Matches;
    for (int i = 0; i < array.Size(); i++) {
        // add match (array[i])
        last10Matches.push_back(array[i].GetString());
    }
    int count = 0;

    int totalDeaths = 0, totalKills = 0, totalAssists = 0, totalCS = 0, totalMinutes = 0, totalWins = 0;

    MultiLineLabelList * list = (MultiLineLabelList*) Manager::instance->getScreen(2)->getElements()[1];
    list->clear();
    for (string matchID : last10Matches) {
        count++;
        Match* match = getMatch(matchID);
        if (match == nullptr) {
            match = loadMatch(matchID);
        }
        MatchPlayer* matchPlayer = match->getTarget(player);
        totalDeaths += matchPlayer->getDeaths();
        totalKills += matchPlayer->getKills();
        totalAssists += matchPlayer->getAssists();
        totalMinutes += getMinutes(match->getDuration());
        totalCS += matchPlayer->getCreepScore();
        if (match->didWin(matchPlayer)) totalWins++;
        Label * label = (Label*) Manager::instance->getCurrentScreen()->getElements()[0];
        label->setText("Loading "+to_string(count)+"/13 matches");
        Manager::instance->draw();
        Manager::instance->refreshScreen();
        MultiLineLabel* matchReport = new MultiLineLabel(Manager::instance->utility->getMatchReport(match, matchPlayer));
        matchReport->setHiddenValue(matchID);
        list->addLabel(matchReport);
    }
    LabelList * informationList = (LabelList*) Manager::instance->getScreen(2)->getElements()[3];
    informationList->clear();
    informationList->addLabel("Total Kills: " + to_string(totalKills));
    informationList->addLabel("Total Deaths: " + to_string(totalDeaths));
    informationList->addLabel("Total Assists: " + to_string(totalAssists));
    informationList->addLabel("Total CS: " + to_string(totalCS));
    informationList->addLabel("Total Victories: "+to_string(totalWins));
    informationList->addLabel("-----------------------------------");
    informationList->addLabel("Average Kills: " + to_string(totalKills / (float) count));
    informationList->addLabel("Average Deaths: " + to_string(totalDeaths / (float) count));
    informationList->addLabel("Average Assists: " + to_string(totalAssists / (float) count));
    informationList->addLabel("Average CS/min: "+ to_string(totalCS / (float) totalMinutes));
    informationList->addLabel("-----------------------------------");
    informationList->addLabel("Average Duration: "+to_string((float) totalMinutes / count)+" minutes");
    float winRate = totalWins / (float) count;

    winRate = roundf(winRate*100.0) ;
    std::ostringstream oss;
    oss << std::setprecision(2) << std::noshowpoint << winRate;
    std::string str = oss.str();

    informationList->addLabel("Win rate: "+str+"%");
    
    return;
}

void QuitButton::execute() {
    Manager::instance->running = false;
}