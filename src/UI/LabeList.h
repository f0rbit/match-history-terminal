#ifndef LABELLIST_H
#define LABELLIST_H

#include <string>
#include <vector>

#include "Boxed.h"

// list of strings
class LabelList : public Boxed {
   public:
    LabelList(int x, int y, int width, int height) : Boxed(width, height) {
        setX(x);
        setY(y);
    }
    void addLabel(std::string str) { this->labels.push_back(str); };  // adds a string
    std::vector<std::string> getLabels() { return labels; };          // returns every string
    virtual void draw(WINDOW* window) override;
    std::string getTitle() { return this->title; };
    void setTitle(std::string title) { this->title = title; };
    void clear() { this->labels.clear(); };  // clears the strings
   protected:
    std::vector<std::string> labels;  // stores all the strings
    std::string title;                // optional title to be drawn above the list.
};

#endif