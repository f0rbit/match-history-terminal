#include "UrlReader.h"

#include <assert.h>
#include <string.h>

#include <iostream>

#include "../../include/curl/curl.h"

const std::string UrlReader::API_KEY = "{ENTER YOUR API KEY HERE}";  // needs to be regenerated every 24 hours.
/**
 * Common Code adapted from:
 * https://www.geeksforgeeks.org/urlify-given-string-replace-spaces/
 * Accessed 27.05.2021
 * 
 */
std::string decode(std::string name) {  // count spaces and find current length
    char str[name.length() + 20];
    strcpy(str, name.c_str());
    int space_count = 0, i;
    for (i = 0; str[i]; i++) {
        if (str[i] == ' ')
            space_count++;
    }
    // Remove trailing spaces
    while (str[i - 1] == ' ') {
        space_count--;
        i--;
    }

    // Find new length.
    int new_length = i + space_count * 2 + 1;

    // New length must be smaller than length
    // of string provided.
    assert(new_length < 1001);

    // Start filling character from end
    int index = new_length - 1;

    // Fill string termination.
    str[index--] = '\0';

    // Fill rest of the string from end
    for (int j = i - 1; j >= 0; j--) {
        // inserts %20 in place of space
        if (str[j] == ' ') {
            str[index] = '0';
            str[index - 1] = '2';
            str[index - 2] = '%';
            index = index - 3;
        } else {
            str[index] = str[j];
            index--;
        }
    }

    return str;
}

/**
 * Common CURL script adapted from:
 * https://gist.github.com/alghanmi/c5d7b761b2c9ab199157
 * Accessed: 23.04.2021
 * 
 */
static size_t writeCallBack(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

// reads the url with the api_key appended automatically
std::string UrlReader::read(std::string url, bool raw) {
    CURL* curl;
    CURLcode res;
    curl = curl_easy_init();
    std::string readBuffer;

    if (curl) {
        if (raw) {
            curl_easy_setopt(curl, CURLOPT_URL, decode(url).c_str());
        } else {
            curl_easy_setopt(curl, CURLOPT_URL, decode(url + "?api_key=" + API_KEY).c_str());
        }
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallBack);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}
// gets a json document from a given URL
Document UrlReader::loadDoc(std::string URL, bool raw) {
    UrlReader reader = UrlReader();
    std::string response = reader.read(URL, raw);
    Document doc;
    doc.Parse(response.c_str());
    return doc;
}