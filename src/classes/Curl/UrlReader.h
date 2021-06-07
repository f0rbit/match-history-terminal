#ifndef URL_READER_H
#define URL_READER_H
#include <iostream>
#include <string>

#include "../../include/rapidjson/document.h"
#include "../../include/rapidjson/rapidjson.h"
using namespace rapidjson;

class UrlReader {
   public:
    std::string read(std::string url, bool raw);  // reads a url with the API_KEY appended to the end

    // gets a json document from a given URL
    static Document loadDoc(std::string URL, bool raw);

    static const std::string API_KEY;  // API_KEY assigned in UrlReader.cpp
};
#endif