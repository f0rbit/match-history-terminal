#ifndef MATCHPLAYER_H
#define MATCHPLAYER_H

#include <iostream>
#include <vector>

#include "Player.h"

class MatchPlayer : public Player {
   public:
    MatchPlayer(){};
    ~MatchPlayer(){};
    // -- standard getters and setters -- //
    int getChampLevel() { return this->champLevel; };
    std::string getChampName() { return this->champName; };
    int getKills() { return this->kills; };
    int getAssists() { return this->assists; };
    int getDeaths() { return this->deaths; };
    int getPinkWardsPlaced() { return this->pinkWardsPlaced; };
    int getGoldEarned() { return this->goldEarned; };
    int getVisionScore() { return this->visionScore; };
    std::vector<int> getItems() { return this->items; };
    std::string getLane() { return this->lane; };
    int getCreepScore() { return this->creepScore; };
    int getSpellD() { return this->spellD; };
    int getSpellF() { return this->spellF; };

    void setChampLevel(int level) { this->champLevel = level; };
    void setChampName(std::string name) { this->champName = name; };
    void setKills(int kills) { this->kills = kills; };
    void setDeaths(int deaths) { this->deaths = deaths; };
    void setAssists(int assists) { this->assists = assists; };
    void setPinkWardsPlaced(int pinks) { this->pinkWardsPlaced = pinks; };
    void setGoldEarned(int gold) { this->goldEarned = gold; };
    void setVisionScore(int vs) { this->visionScore = vs; };
    void setItems(std::vector<int> i) { this->items = i; };
    void setLane(std::string lane) { this->lane = lane; };
    void setCreepScore(int cs) { this->creepScore = cs; };
    void setSpellD(int id) { this->spellD = id; };
    void setSpellF(int id) { this->spellF = id; };

    void addItem(int itemID) { items.emplace_back(itemID); }

    // functions
    //void load(GenericObject<false, Value> json);  // loads a player from a given json object
    std::vector<std::string> getDisplayCard();  // gets a formatted vector of strings showing details of this player.

    // debug print function.
    virtual void print() {
        std::cout << this->champName << ": " << this->kills << "/" << this->deaths << "/" << this->assists << ", " << this->lane << " lane" << std::endl;
    };

   private:
    int champLevel;
    std::string champName;

    int kills;
    int assists;
    int deaths;

    int pinkWardsPlaced;
    int goldEarned;
    int visionScore;
    int creepScore;

    int spellD;
    int spellF;

    std::vector<int> items = {};
    std::string lane;
};

#endif