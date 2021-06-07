#ifndef PLAYER_H
#define PLAYER_H

#include <string>

/** PURE ABSTRACT CLASS **/
// this class is inherited by MatchPlayer and UserPlayer
class Player {
   public:
    std::string getSummonerName() { return this->summonerName; };
    std::string getID() { return this->id; };
    int getLevel() { return this->level; };

    void setSummonerName(std::string name) { this->summonerName = name; };
    void setID(std::string id) { this->id = id; };
    void setLevel(int level) { this->level = level; };

    virtual void print() = 0;  // all subclasses should implement a print() function
   protected:
    std::string summonerName;
    int level;
    std::string id;
};

#endif