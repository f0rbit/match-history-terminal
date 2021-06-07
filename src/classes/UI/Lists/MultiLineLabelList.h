#ifndef MUTLILINELABELLIST_H
#define MUTLILINELABELLIST_H

#include <string>
#include <vector>

#include "../Labels/MultiLineLabel.h"
#include "SelectableList.h"

// a list of MutliLineLabels
class MultiLineLabelList : public SelectableList {
   public:
    MultiLineLabelList(int x, int y, int width, int height) : SelectableList(x, y, width, height) {}

    void addLabel(MultiLineLabel* label) { this->labels.emplace_back(label); };  // adds label to vector
    std::vector<MultiLineLabel*> getLabels() { return this->labels; };
    std::string getTitle() { return this->title; };
    void setTitle(std::string title) { this->title = title; };
    virtual void draw() override;                            // override abstract function
    void move(bool up);                                      // moves up or down the list
    void clear() { labels.clear(); };                        // clears the labels vector
    int getSelectedLabel() { return this->selectedLabel; };  // gets the index of the currently selected label
   protected:
    std::vector<MultiLineLabel*> labels;  // stores all the labels
    std::string title;                    // optional title, drawn above the list
    int selectedLabel = 0;                // the currently selected label
};

#endif