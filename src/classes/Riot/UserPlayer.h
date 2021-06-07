#ifndef USERPLAYER_H
#define USERPLAYER_H

#include <iostream>
#include <vector>

#include "Player.h"

// the class that the user is searching data for ( our main summoner )
class UserPlayer : public Player {
   public:
    // -- Getters and setters -- //
    std::string getPUUID() { return this->pUUID; };
    void setPUUID(std::string puuid) { this->pUUID = puuid; };
    // -- functions -- //
    std::vector<std::string> getMatchHistory() { return this->matchHistory; };  // gets a list of match ids
    virtual void print() {
        std::cout << "name: " << this->getSummonerName() << ", id: " << this->getID() << ", level: " << this->getLevel() << ", puuid: " << this->getPUUID() << std::endl;
    };

   private:
    std::string pUUID;
    std::vector<std::string> matchHistory;
};

#endif