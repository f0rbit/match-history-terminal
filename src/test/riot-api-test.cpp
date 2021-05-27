#include <iostream>
#include "../include/rapidjson/writer.h"
#include "../include/rapidjson/document.h"
#include "../include/rapidjson/stringbuffer.h"
#include "../include/curl/curl.h"
#include <string>
#include "../UrlReader.h"
using namespace std;
using namespace rapidjson;


static size_t writeCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

string read(const std::string& url)
{
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string readBuffer;

    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}


int main() {

    UrlReader urlReader = UrlReader();
    string SUMMONER_NAME = "TEST";

    string REQUEST_URL = "https://oc1.api.riotgames.com/lol/summoner/v4/summoners/by-name/"+SUMMONER_NAME+"?api_key="+UrlReader::API_KEY;

    cout << read(REQUEST_URL) << endl << endl;

    cout << urlReader.read("https://oc1.api.riotgames.com/lol/summoner/v4/summoners/by-name/"+SUMMONER_NAME) << endl;

    


}