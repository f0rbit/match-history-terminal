#ifndef RIOT_CLASSES_H
#define RIOT_CLASSES_H
#include <string.h>
#include <ctime>
#include <map>
#include <vector>
#include <iostream>
#include "include/rapidjson/document.h"
using std::string;
using std::vector;
using std::cout;
using std::endl;
using namespace rapidjson;

/** PURE ABSTRACT CLASS **/
// this class is inherited by MatchPlayer and UserPlayer
class Player {
    public:
        string getSummonerName() { return this->summonerName; };
        string getID() { return this->id; };
        int getLevel() { return this->level; };

        void setSummonerName(string name) { this->summonerName = name;};
        void setID(string id) { this->id = id;};
        void setLevel(int level) { this->level = level;};

        virtual void print() = 0; // all subclasses should implement a print() function
    protected:
        string summonerName;
        int level;
        string id;
};

class MatchPlayer : public Player {
    public:
        MatchPlayer() {};
        ~MatchPlayer() {};
        // -- standard getters and setters -- //
        int getChampLevel() { return this->champLevel; };
        string getChampName() { return this->champName; };
        int getKills() { return this->kills; };
        int getAssists() { return this->assists; };
        int getDeaths() { return this->deaths; };
        int getPinkWardsPlaced() { return this->pinkWardsPlaced; };
        int getGoldEarned() { return this->goldEarned; };
        int getVisionScore() { return this->visionScore; };
        vector<int> getItems() { return this->items; };
        string getLane() { return this->lane; };
        int getCreepScore() { return this->creepScore; };
        int getSpellD() { return this->spellD; };
        int getSpellF() { return this->spellF; };

        void setChampLevel(int level) { this->champLevel = level; };
        void setChampName(string name) { this->champName = name; };
        void setKills(int kills) { this->kills = kills; };
        void setDeaths(int deaths) { this->deaths = deaths; };
        void setAssists(int assists) { this->assists = assists; };
        void setPinkWardsPlaced(int pinks) { this->pinkWardsPlaced = pinks; };
        void setGoldEarned(int gold) { this->goldEarned = gold; };
        void setVisionScore(int vs) { this->visionScore = vs; };
        void setItems(vector<int> i) { this->items = i; };
        void setLane(string lane) { this->lane = lane; };
        void setCreepScore(int cs) { this->creepScore = cs; };
        void setSpellD(int id) { this->spellD = id; };
        void setSpellF(int id) { this->spellF = id; };

        // functions
        void load(GenericObject<false, Value> json); // loads a player from a given json object
        vector<string> getDisplayCard(); // gets a formatted vector of strings showing details of this player.

        // debug print function.
        virtual void print() {
            cout << this->champName << ": " << this->kills << "/" << this->deaths << "/" << this->assists << ", " << this->lane << " lane" << endl;
        };

    private:
        int champLevel;
        string champName;

        int kills;
        int assists;
        int deaths;

        int pinkWardsPlaced;
        int goldEarned;
        int visionScore;
        int creepScore;

        int spellD;
        int spellF;

        vector<int> items;
        string lane;

};


// the class that the user is searching data for ( our main summoner )
class UserPlayer : public Player {
    public:
        // -- Getters and setters -- //
        string getPUUID() { return this->pUUID; };
        void setPUUID(string puuid) { this->pUUID = puuid; };
        // -- functions -- //
        vector<string> getMatchHistory() { return this->matchHistory; }; // gets a list of match ids
        virtual void print() { 
            cout << "name: " << this->getSummonerName() << ", id: " << this->getID() << ", level: " << this->getLevel() << ", puuid: " << this->getPUUID() << endl; 
        };
    private:
        string pUUID;
        vector<string> matchHistory;
};

class Team {
    public:
        void setDragonKills(int kills) { this->dragonKills = kills; };
        void setBarons(int barons) { this->barons = barons; };
        void setRiftHeralds(int rifts) { this->riftHeralds = rifts; };
        void setPlayers(vector<MatchPlayer*> players) { this->players = players; };
        void setWin(bool win) { this->win = win; };

        int getDragonKills() { return this->dragonKills; };
        int getBarons() { return this->barons; };
        int getRiftHeralds() { return this->riftHeralds; };
        vector<MatchPlayer*> getPlayers() { return this->players; };
        bool didWin() { return this->win; };

        void addPlayer(MatchPlayer * player) { this->players.push_back(player); };
        void loadData(GenericArray<false, Value> data, int index);

    private:
        int dragonKills;
        int barons;
        int riftHeralds;
        vector<MatchPlayer*> players;
        bool win;
};

/**
 *  Match class stores data about a match
 */
class Match {
    public:
        Match() { this->matchID = "0"; }; 
        Match(string matchID) { this->matchID = matchID; };
        // standard getters and setters
        void setBlueTeam(Team* t) { this->blue = t; };
        void setRedTeam(Team* t) { this->red = t; };
        void setQueueID(int id) { this->queueID = id; };
        void setCreation(long gameCreation) { this->gameCreation = gameCreation; };
        void setDuration(long gameDuration) { this->gameDuration = gameDuration; };

        Team* getBlueTeam() { return this->blue; };
        Team* getRedTeam() { return this->red; };
        int getQueueID() { return this->queueID; };
        string getMatchID() { return this->matchID; };
        long getCreation() { return this->gameCreation; };
        int getDuration() { return this->gameDuration; };

        void load(Document doc);                                // loads the match from a given json document
        MatchPlayer* getTarget(UserPlayer* player);             // gets the MatchUser from a UserPlayer
        int goldGap() {                                         // calculates the gold difference between the blue team and red team
            int blueGold = 0;                                   // will be positive if in favour of blue team, negative if in favour of red team.
            int redGold = 0;
            for (MatchPlayer* player : blue->getPlayers()) {
                blueGold += player->getGoldEarned();
            }
            for (MatchPlayer* player : red->getPlayers()) {
                redGold += player->getGoldEarned();
            }
            return blueGold-redGold;
        }
        bool didWin(MatchPlayer* player) {                      // returns if the specified player won this match.
            for (MatchPlayer* p : blue->getPlayers()) {
                if (p == player) return blue->didWin();
            }
            return red->didWin();
        }

    private:
        Team* blue;
        Team* red;
        int queueID;
        string matchID;
        long gameCreation;
        int gameDuration;
        // cleanup
        ~Match() {
            delete[] blue, red;
        }

};


class Utility {
    //Utility class for converting from raw data
    public: 
        string getItemName(int ID) { return items.at(ID); };
        string getSpellName(int ID) { return spells.at(ID); };
        string getQueueName(int ID);
        void loadItems(GenericObject<false, Value> doc); //Loads JSON containing item List
        void loadSpells(GenericObject<false, Value> doc); // Loads JSON Containing Summoner Spell list
        void loadQueues(GenericArray<false, Value> doc); // Loads JSON containing QueueID's
        bool loadPlayer(UserPlayer * player);
        /* Maps to store all Items/ Spells and Queue names
            Converts from Key to actual name/ description
        */
        std::map<int,string> getSpells() { return this->spells; };
        std::map<int,string> getItems() { return this-> items; };
        std::map<int,string> getQueues(){return this-> queues; };

        vector<string> getMatchReport(Match* match, MatchPlayer* target); // returns the match report
    
        time_t getTime(long ms) { // gets the time from milliseconds
            return ms;
        }
    protected:
        std::map<int,string> items;
        std::map<int,string> spells;
        std::map<int,string> queues;
};
#endif