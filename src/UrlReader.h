#ifndef URL_READER_H
#define URL_READER_H
#include <string>
#include <iostream>
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"
using namespace rapidjson;
using std::string;
class UrlReader {
    public: 
        string read(string url); // reads a url with the API_KEY appended to the end
        string readRaw(string url); // reads a url with nothing appended to the end

        // gets a json document from a given URL
        static Document loadDoc(string URL, bool raw) { 
            UrlReader reader = UrlReader();
            string response = raw ? reader.readRaw(URL) : reader.read(URL); // readRaw based on (bool) raw
            Document doc;
            doc.Parse(response.c_str());
            return doc;
        }
        static const string API_KEY; // API_KEY assigned in UrlReader.cpp
};
#endif