#ifndef MATCH_H
#define MATCH_H

#include "Team.h"
#include "UserPlayer.h"

/**
 *  Match class stores data about a match
 */
class Match {
   public:
    Match() { this->matchID = "0"; };
    Match(std::string matchID) { this->matchID = matchID; };
    // standard getters and setters
    void setBlueTeam(Team* t) { this->blue = t; };
    void setRedTeam(Team* t) { this->red = t; };
    void setQueueID(int id) { this->queueID = id; };
    void setCreation(long gameCreation) { this->gameCreation = gameCreation; };
    void setDuration(long gameDuration) { this->gameDuration = gameDuration; };
    void setMatchID(std::string id) { this->matchID = id; };

    Team* getBlueTeam() { return this->blue; };
    Team* getRedTeam() { return this->red; };
    int getQueueID() { return this->queueID; };
    std::string getMatchID() { return this->matchID; };
    long getCreation() { return this->gameCreation; };
    int getDuration() { return this->gameDuration; };

    //void load(Document doc);                     // loads the match from a given json document
    MatchPlayer* getTarget(UserPlayer* player);  // gets the MatchUser from a UserPlayer
    int goldGap() {                              // calculates the gold difference between the blue team and red team
        int blueGold = 0;                        // will be positive if in favour of blue team, negative if in favour of red team.
        int redGold = 0;
        for (MatchPlayer* player : blue->getPlayers()) {
            blueGold += player->getGoldEarned();
        }
        for (MatchPlayer* player : red->getPlayers()) {
            redGold += player->getGoldEarned();
        }
        return blueGold - redGold;
    }
    bool didWin(MatchPlayer* player) {  // returns if the specified player won this match.
        for (MatchPlayer* p : blue->getPlayers()) {
            if (p == player) return blue->didWin();
        }
        return red->didWin();
    }

   private:
    Team* blue;
    Team* red;
    int queueID;
    std::string matchID;
    long gameCreation;
    int gameDuration;
    // cleanup
    ~Match() {
        delete[] blue, red;
    }
};
#endif