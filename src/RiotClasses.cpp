#include "RiotClasses.h"
#include "UrlReader.h"
#include "Manager.h"
#include "include/rapidjson/document.h"
#include <ctime>
#include <iomanip>
#include <fstream>
// DEFINING User Player

using std::string;
using std::to_string;
using std::put_time;
using namespace rapidjson;

void MatchPlayer::load(GenericObject<false, Value> doc) {
    setChampLevel(doc["champLevel"].GetInt());
    setChampName(doc["championName"].GetString());
    setKills(doc["kills"].GetInt());
    setDeaths(doc["deaths"].GetInt());
    setAssists(doc["assists"].GetInt());
    setPinkWardsPlaced(doc["detectorWardsPlaced"].GetInt());
    setGoldEarned(doc["goldEarned"].GetInt());
    setVisionScore(doc["visionScore"].GetInt());
    setLane(doc["lane"].GetString());
    setID(doc["summonerId"].GetString());
    setSummonerName(doc["summonerName"].GetString());
    setLevel(doc["summonerLevel"].GetInt());
    setCreepScore(doc["totalMinionsKilled"].GetInt());
    setSpellD(doc["summoner1Id"].GetInt());
    setSpellF(doc["summoner2Id"].GetInt());
    
    for (int i = 0; i < 6; i++) {
        string itemx = "item" + to_string(i);
        int itemID = doc[itemx.c_str()].GetInt();
        this->items.push_back(itemID);
    }
}

vector<string> MatchPlayer::getDisplayCard() {
    vector<string> strings;
    strings.push_back(getChampName()+" - \""+getSummonerName()+"\"");
    strings.push_back("["+to_string(getKills())+"/"+to_string(getDeaths())+"/"+to_string(getAssists())+"] - Level "+to_string(getChampLevel()));
    strings.push_back(to_string(getCreepScore())+" cs - "+to_string(getVisionScore())+" vision score");
    strings.push_back(to_string(getGoldEarned())+" gold - "+to_string(getPinkWardsPlaced()) + " control wards");
    return strings;
}

MatchPlayer* Match::getTarget(UserPlayer* player) {
    //cout << "GETTING TARGET" << endl;
    for (int i = 0; i < 5; i++) {
        MatchPlayer* red = getRedTeam()->getPlayers().at(i);
        MatchPlayer* blue = getBlueTeam()->getPlayers().at(i);
        if (red->getID() == player->getID()) return red;
        if (blue->getID() == player->getID()) return blue;
    }
    assert(false);
}

string getDate(time_t time) {
    auto tm = *std::localtime(&time);
    std::ostringstream oss;
    oss << put_time(&tm, "%d/%m/%Y %H:%M");
    return oss.str();
}

string getTimeBreakdown(long milliseconds) {
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    int minutes = seconds / 60;
    seconds %= 60;

    int hours = minutes / 60;
    minutes %= 60;

    return ((hours > 0) ? hours+"h " : "") + to_string(minutes) + "m " + to_string(seconds)+"s";
}

// gets the formatted match report for a player withing a match
vector<string> Utility::getMatchReport(Match* match, MatchPlayer* target) {
    return {
        (target->getChampName() + "(Level "+ to_string(target->getChampLevel())+") - "+target->getLane()+" Lane"),
        ("KDA ["+to_string(target->getKills())+"/"+to_string(target->getDeaths())+"/"+to_string(target->getAssists())+"]  Date - "+getDate(static_cast<time_t>(match->getCreation() / 1000))+"  Duration - "+getTimeBreakdown(match->getDuration()))
    };
}

// loads match data from a json document
void Match::load(Document doc) {
    // load teams
    // loop through each entry of participants and make a match player for each
    GenericArray<false, Value> participants = doc["info"]["participants"].GetArray();\
    // create the two teams
    setBlueTeam((new Team()));
    setRedTeam((new Team()));
    // add players to teams
    for (int i = 0; i < 10; i++) {
        // participant index i
        MatchPlayer *player = new MatchPlayer();
        player->load(participants[i].GetObject()); // load data into player
        if (i < 5) {
            getBlueTeam()->addPlayer(player);
        } else {
            getRedTeam()->addPlayer(player);
        }
    }

    // load data about the match    
    setQueueID(doc["info"]["queueId"].GetInt());
    setDuration(doc["info"]["gameDuration"].GetInt());
    setCreation(doc["info"]["gameCreation"].GetInt64());

    // load data about the two teams.
    GenericArray<false, Value> teams = doc["info"]["teams"].GetArray();
    getBlueTeam()->loadData(teams, 0);
    getRedTeam()->loadData(teams, 1);
}


// get objective data for a team.
void Team::loadData(GenericArray<false, Value> array, int index) {
    setWin(array[index]["win"].GetBool());
    setBarons(array[index]["objectives"]["baron"]["kills"].GetInt());
    setDragonKills(array[index]["objectives"]["dragon"]["kills"].GetInt());
}

// Utility Classes
//Load Summoner Spells
void Utility::loadSpells(GenericObject<false, Value> doc) {
    //Iterate through Object List and Grab keys and Names
    for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
        string name = itr->value.GetObject()["name"].GetString();
        string id = itr->value.GetObject()["key"].GetString();
        int spellID = stoi(id); // String to Int
        this->spells.emplace(spellID, name); // store into Map
    }
}
//Load Items
void Utility::loadItems(GenericObject<false,Value> doc ){
    //Iterate through Object List
    for(Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr){
        string id = itr->name.GetString();
        string name = itr->value.GetObject()["name"].GetString();
        int itemID = stoi(id); // String to Int
        items.emplace(itemID,name); // store data in map
    } 
}

void Utility::loadQueues(GenericArray<false,Value> doc){
    for (int i=1;i<doc.Size();i++){ // Iterate through array
        int ID = doc[i].GetObject()["queueId"].GetInt();
        string name = doc[i].GetObject()["description"].GetString();
        queues.emplace(ID,name); // store ID and Descriptions in map
    }
}


// loads player data
// returns false if player name isn't valid
bool Utility::loadPlayer(UserPlayer* player) {
    // make api call using summoner name
    string URL = "https://oc1.api.riotgames.com/lol/summoner/v4/summoners/by-name/"+player->getSummonerName(); 
    //API call for Summoner Name - Returns PUUID, ID, acc Level
    Manager::output << "Calling URL" << URL << endl;
    UrlReader reader = UrlReader();
    string response = reader.read(URL);
    Manager::output << "RESPONSE: " << endl << response << endl;

    // Parse the JSON string into json document
    if (response.empty()) {
        Manager::output << "NO RESPONSE!" << endl;
        Manager::output.flush();
        return false;
    }
    Document json;
    json.Parse(response.c_str());

    Manager::output << response << endl;

    // checks to see if summoner is valid
    if (json.HasMember("status")) {
        if (json["status"]["status_code"].GetInt() == 404) {
            // error
            Manager::output << "STATUS CODE 404" << endl;
            Manager::output.flush();
            return false;
        } else if (json["status"]["status_code"].GetInt() == 403) {
            Manager::output << "STATUS CODE 403" << endl;
            Manager::output.flush();
            return false;
        }
    }
    Manager::output.flush();
    
    // set data about player.
    player->setID(json["id"].GetString());
    player->setPUUID(json["puuid"].GetString());
    player->setLevel(json["summonerLevel"].GetInt());

    return true;

}