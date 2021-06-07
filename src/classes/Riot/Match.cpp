#include "Match.h"

#include "../../include/rapidjson/document.h"

using namespace rapidjson;

MatchPlayer* Match::getTarget(UserPlayer* player) {
    //cout << "GETTING TARGET" << endl;
    for (int i = 0; i < 5; i++) {
        MatchPlayer* red = getRedTeam()->getPlayers().at(i);
        MatchPlayer* blue = getBlueTeam()->getPlayers().at(i);
        if (red->getID() == player->getID()) return red;
        if (blue->getID() == player->getID()) return blue;
    }
    return nullptr;
}