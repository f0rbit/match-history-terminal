#ifndef TEAM_H
#define TEAM_H

#include <vector>

#include "MatchPlayer.h"
class Team {
   public:
    void setDragonKills(int kills) { this->dragonKills = kills; };
    void setBarons(int barons) { this->barons = barons; };
    void setRiftHeralds(int rifts) { this->riftHeralds = rifts; };
    void setPlayers(std::vector<MatchPlayer*> players) { this->players = players; };
    void setWin(bool win) { this->win = win; };

    int getDragonKills() { return this->dragonKills; };
    int getBarons() { return this->barons; };
    int getRiftHeralds() { return this->riftHeralds; };
    std::vector<MatchPlayer*> getPlayers() { return this->players; };
    bool didWin() { return this->win; };

    void addPlayer(MatchPlayer* player) { players.emplace_back(player); };
    //void loadData(GenericArray<false, Value> data, int index);

   private:
    int dragonKills;
    int barons;
    int riftHeralds;
    std::vector<MatchPlayer*> players;
    bool win;
};

#endif