#ifndef UTILITY_H
#define UTLIITY_H

#include <map>

#include "../../include/rapidjson/document.h"
#include "Match.h"

using namespace rapidjson;

class Utility {
    //Utility class for converting from raw data
   public:
    std::string getItemName(int ID) { return items.at(ID); };
    std::string getSpellName(int ID) { return spells.at(ID); };
    std::string getQueueName(int ID);
    void loadItems(GenericObject<false, Value> doc);   //Loads JSON containing item List
    void loadSpells(GenericObject<false, Value> doc);  // Loads JSON Containing Summoner Spell list
    void loadQueues(GenericArray<false, Value> doc);   // Loads JSON containing QueueID's
    bool loadPlayer(UserPlayer* player);

    bool loadData();

    Team* loadTeam(GenericArray<false, Value> array, int index);
    Match* loadMatch(Document doc);
    MatchPlayer* loadPlayer(GenericObject<false, Value> doc);
    /* Maps to store all Items/ Spells and Queue names
            Converts from Key to actual name/ description
        */
    std::map<int, std::string> getSpells() { return this->spells; };
    std::map<int, std::string> getItems() { return this->items; };
    std::map<int, std::string> getQueues() { return this->queues; };

    std::vector<std::string> getMatchReport(Match* match, MatchPlayer* target);  // returns the match report

    time_t getTime(long ms) {  // gets the time from milliseconds
        return ms;
    }

    static int getMinutes(long ms);
    static std::string getDate(time_t time);
    static std::string getTimeBreakdown(long ms);

   protected:
    std::map<int, std::string> items = {};
    std::map<int, std::string> spells;
    std::map<int, std::string> queues;
};

#endif