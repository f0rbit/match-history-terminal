#ifndef MATCHSTORAGE_H
#define MATCHSTORAGE_H

#include <vector>

#include "../Riot/Match.h"
/**
 * Stores all the loaded matches
 */
class MatchStorage {
   public:
    // gets the pointer of an already loaded match given its match ID
    // if it can't find any it returns nullptr.
    Match* getMatch(std::string matchID) {
        for (Match* match : loadedMatches) {
            if (match->getMatchID() == matchID) return match;
        }
        return nullptr;
    }

    // loads a match from Riot's API given its matchID
    // matchID's are obtained from loadHistory()
    Match* loadMatch(std::string matchID);
    // loads the match history of a given player
    void loadHistory(UserPlayer* player);

   private:
    // stores all the loaded matches
    std::vector<Match*> loadedMatches;
};

#endif