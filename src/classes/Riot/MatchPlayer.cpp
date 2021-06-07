#include "MatchPlayer.h"

using std::to_string;

std::vector<std::string> MatchPlayer::getDisplayCard() {
    std::vector<std::string> strings;
    strings.push_back(getChampName() + " - \"" + getSummonerName() + "\"");
    strings.push_back("[" + to_string(getKills()) + "/" + to_string(getDeaths()) + "/" + to_string(getAssists()) + "] - Level " + to_string(getChampLevel()));
    strings.push_back(to_string(getCreepScore()) + " cs - " + to_string(getVisionScore()) + " vision score");
    strings.push_back(to_string(getGoldEarned()) + " gold - " + to_string(getPinkWardsPlaced()) + " control wards");
    return strings;
}