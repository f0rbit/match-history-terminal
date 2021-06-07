#include "Utility.h"

#include <ctime>
#include <fstream>
#include <iomanip>
#include <sstream>

#include "../Curl/UrlReader.h"

using std::string;
using std::to_string;
using std::vector;

bool Utility::loadData() {
    string spellsURL = "https://ddragon.leagueoflegends.com/cdn/11.10.1/data/en_US/summoner.json";
    string itemsURL = "https://ddragon.leagueoflegends.com/cdn/11.10.1/data/en_US/item.json";
    string queuesURL = "http://static.developer.riotgames.com/docs/lol/queues.json";
    this->loadSpells(UrlReader::loadDoc(spellsURL, false)["data"].GetObject());
    this->loadItems(UrlReader::loadDoc(itemsURL, false)["data"].GetObject());
    this->loadQueues(UrlReader::loadDoc(queuesURL, false).GetArray());

    return true;
}

string Utility::getDate(time_t time) {
    auto tm = *std::localtime(&time);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d/%m/%Y %H:%M");
    return oss.str();
}

string Utility::getTimeBreakdown(long milliseconds) {
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    int minutes = seconds / 60;
    seconds %= 60;

    int hours = minutes / 60;
    minutes %= 60;

    return ((hours > 0) ? hours + "h " : "") + to_string(minutes) + "m " + to_string(seconds) + "s";
}

// gets the amount of minutes from millisecond duration
int Utility::getMinutes(long milliseconds) {
    int seconds = milliseconds / 1000;
    milliseconds %= 1000;

    int minutes = seconds / 60;
    seconds %= 60;
    return minutes;
}

// get objective data for a team.
Team* Utility::loadTeam(GenericArray<false, Value> array, int index) {
    Team* team = new Team();
    team->setWin(array[index]["win"].GetBool());
    team->setBarons(array[index]["objectives"]["baron"]["kills"].GetInt());
    team->setDragonKills(array[index]["objectives"]["dragon"]["kills"].GetInt());
    return team;
}

// Utility Classes
//Load Summoner Spells
void Utility::loadSpells(GenericObject<false, Value> doc) {
    //Iterate through Object List and Grab keys and Names
    for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
        string name = itr->value.GetObject()["name"].GetString();
        string id = itr->value.GetObject()["key"].GetString();
        int spellID = stoi(id);               // String to Int
        this->spells.emplace(spellID, name);  // store into Map
    }
}
//Load Items
void Utility::loadItems(GenericObject<false, Value> doc) {
    //Iterate through Object List
    for (Value::ConstMemberIterator itr = doc.MemberBegin(); itr != doc.MemberEnd(); ++itr) {
        string id = itr->name.GetString();
        string name = itr->value.GetObject()["name"].GetString();
        int itemID = stoi(id);        // String to Int
        items.emplace(itemID, name);  // store data in map
    }
}

void Utility::loadQueues(GenericArray<false, Value> doc) {
    for (int i = 1; i < doc.Size(); i++) {  // Iterate through array
        int ID = doc[i].GetObject()["queueId"].GetInt();
        string name = doc[i].GetObject()["description"].GetString();
        queues.emplace(ID, name);  // store ID and Descriptions in map
    }
}

// loads player data
// returns false if player name isn't valid
bool Utility::loadPlayer(UserPlayer* player) {
    // make api call using summoner name
    string URL = "https://oc1.api.riotgames.com/lol/summoner/v4/summoners/by-name/" + player->getSummonerName();
    //API call for Summoner Name - Returns PUUID, ID, acc Level
    UrlReader reader = UrlReader();
    string response = reader.read(URL, false);

    // Parse the JSON string into json document
    if (response.empty()) {
        return false;
    }
    Document json;
    json.Parse(response.c_str());

    // checks to see if summoner is valid
    if (json.HasMember("status")) {
        int id = json["status"]["status_code"].GetInt();
        if (id == 404 || id == 403) {
            return false;
        }
    }
    // set data about player->
    player->setID(json["id"].GetString());
    player->setPUUID(json["puuid"].GetString());
    player->setLevel(json["summonerLevel"].GetInt());

    return true;
}
// gets the formatted match report for a player withing a match
vector<string> Utility::getMatchReport(Match* match, MatchPlayer* target) {
    return {
        (target->getChampName() + "(Level " + to_string(target->getChampLevel()) + ") - " + target->getLane() + " Lane"),
        ("KDA [" + to_string(target->getKills()) + "/" + to_string(target->getDeaths()) + "/" + to_string(target->getAssists()) + "]  Date - " + getDate(static_cast<time_t>(match->getCreation() / 1000)) + "  Duration - " + getTimeBreakdown(match->getDuration()))};
}

MatchPlayer* Utility::loadPlayer(GenericObject<false, Value> doc) {
    MatchPlayer* player = new MatchPlayer();
    player->setChampLevel(doc["champLevel"].GetInt());
    player->setChampName(doc["championName"].GetString());
    player->setKills(doc["kills"].GetInt());
    player->setDeaths(doc["deaths"].GetInt());
    player->setAssists(doc["assists"].GetInt());
    player->setPinkWardsPlaced(doc["detectorWardsPlaced"].GetInt());
    player->setGoldEarned(doc["goldEarned"].GetInt());
    player->setVisionScore(doc["visionScore"].GetInt());
    player->setLane(doc["lane"].GetString());
    player->setID(doc["summonerId"].GetString());
    player->setSummonerName(doc["summonerName"].GetString());
    player->setLevel(doc["summonerLevel"].GetInt());
    player->setCreepScore(doc["totalMinionsKilled"].GetInt());
    player->setSpellD(doc["summoner1Id"].GetInt());
    player->setSpellF(doc["summoner2Id"].GetInt());

    for (int i = 0; i < 6; i++) {
        string itemx = "item" + to_string(i);
        int itemID = doc[itemx.c_str()].GetInt();
        player->addItem(itemID);
    }
    return player;
}

// loads match data from a json document
Match* Utility::loadMatch(Document doc) {
    Match* match = new Match();
    // load teams
    // loop through each entry of participants and make a match player for each
    GenericArray<false, Value> participants = doc["info"]["participants"].GetArray();  // create the two teams
    // load data about the two teams.
    GenericArray<false, Value> teams = doc["info"]["teams"].GetArray();
    match->setBlueTeam(loadTeam(teams, 0));
    match->setRedTeam(loadTeam(teams, 1));

    // add players to teams
    for (int i = 0; i < 10; i++) {
        // participant index i
        MatchPlayer* player = loadPlayer(participants[i].GetObject());
        if (i < 5) {
            match->getBlueTeam()->addPlayer(player);
        } else {
            match->getRedTeam()->addPlayer(player);
        }
    }

    // load data about the match
    match->setQueueID(doc["info"]["queueId"].GetInt());
    match->setDuration(doc["info"]["gameDuration"].GetInt());
    match->setCreation(doc["info"]["gameCreation"].GetInt64());

    return match;
}