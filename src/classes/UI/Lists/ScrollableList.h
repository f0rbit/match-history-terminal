#ifndef SCROLLABLELIST_H
#define SCROLLABELLIST_H

#include "SelectableList.h"

// list that is scrollable.
class ScrollableList : public SelectableList {
   public:
    ScrollableList(int x, int y, int width, int height) : SelectableList(x, y, width, height) {}
    virtual void draw() override;
    virtual void move(bool up) override;

   protected:
    int yOffset = 0;
};

#endif