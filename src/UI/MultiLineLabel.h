#ifndef MUTLILINELABEL_H
#define MUTLILINELABEL_H
#include <ncurses.h>

#include <string>
#include <vector>
// a label that has multiple lines.
// isn't classified as an element as is only used within
// MutliLineLabelList class
class MultiLineLabel {
   public:
    MultiLineLabel(std::vector<std::string> list) {
        this->strings = list;
    }
    std::vector<std::string> getStrings() { return this->strings; };
    void addString(std::string str) { strings.push_back(str); };
    void draw(WINDOW* window, int x, int y);
    std::string getHiddenValue() { return this->hiddenValue; };
    void setHiddenValue(std::string value) { this->hiddenValue = value; };

   protected:
    std::vector<std::string> strings;  // the strings to be drawn
    std::string hiddenValue;           // hidden value can store data.
};
#endif