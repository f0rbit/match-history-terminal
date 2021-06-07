#include "MatchStorage.h"

#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>

#include "../../include/rapidjson/document.h"
#include "../Curl/UrlReader.h"
#include "../UI/Labels/Label.h"
#include "../UI/Labels/MultiLineLabel.h"
#include "../UI/Lists/MultiLineLabelList.h"
#include "Controller.h"

using std::to_string;
using namespace rapidjson;

// loads a match from Riot's API given its matchID
// matchID's are obtained from loadHistory()
Match* MatchStorage::loadMatch(std::string matchID) {
    std::string URL = "https://americas.api.riotgames.com/lol/match/v5/matches/" + matchID;
    Match* match = Controller::instance->utility->loadMatch(UrlReader::loadDoc(URL, false));
    match->setMatchID(matchID);
    loadedMatches.push_back(match);
    return match;
}

void MatchStorage::loadHistory(UserPlayer* player) {
    std::string URL = "https://americas.api.riotgames.com/lol/match/v5/matches/by-puuid/" + player->getPUUID() + "/ids?start=0&count=13&api_key=" + UrlReader::API_KEY;
    Document doc = UrlReader::loadDoc(URL, true);
    GenericArray<false, Value> array = doc.GetArray();
    std::vector<std::string> last10Matches;
    for (int i = 0; i < array.Size(); i++) {
        // add match (array[i])
        last10Matches.push_back(array[i].GetString());
    }
    int count = 0;

    int totalDeaths = 0, totalKills = 0, totalAssists = 0, totalCS = 0, totalMinutes = 0, totalWins = 0;

    MultiLineLabelList* list = (MultiLineLabelList*)DisplayManager::instance->getScreen(2)->getElements()[1];
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
        totalMinutes += Utility::getMinutes(match->getDuration());
        totalCS += matchPlayer->getCreepScore();
        if (match->didWin(matchPlayer)) totalWins++;
        Label* label = (Label*)DisplayManager::instance->getCurrentScreen()->getElements()[0];
        label->setText("Loading " + to_string(count) + "/13 matches");
        DisplayManager::instance->draw();
        DisplayManager::instance->refreshScreen();
        MultiLineLabel* matchReport = new MultiLineLabel(Controller::instance->utility->getMatchReport(match, matchPlayer));
        matchReport->setHiddenValue(matchID);
        list->addLabel(matchReport);
    }
    LabelList* informationList = (LabelList*)DisplayManager::instance->getScreen(2)->getElements()[3];
    informationList->clear();
    informationList->addLabel("Total Kills: " + to_string(totalKills));
    informationList->addLabel("Total Deaths: " + to_string(totalDeaths));
    informationList->addLabel("Total Assists: " + to_string(totalAssists));
    informationList->addLabel("Total CS: " + to_string(totalCS));
    informationList->addLabel("Total Victories: " + to_string(totalWins));
    informationList->addLabel("-----------------------------------");
    informationList->addLabel("Average Kills: " + to_string(totalKills / (float)count));
    informationList->addLabel("Average Deaths: " + to_string(totalDeaths / (float)count));
    informationList->addLabel("Average Assists: " + to_string(totalAssists / (float)count));
    informationList->addLabel("Average CS/min: " + to_string(totalCS / (float)totalMinutes));
    informationList->addLabel("-----------------------------------");
    informationList->addLabel("Average Duration: " + to_string((float)totalMinutes / count) + " minutes");
    float winRate = totalWins / (float)count;

    winRate = std::roundf(winRate * 100.0);
    std::ostringstream oss;
    oss << std::setprecision(2) << std::noshowpoint << winRate;
    std::string str = oss.str();

    informationList->addLabel("Win rate: " + str + "%");

    return;
}