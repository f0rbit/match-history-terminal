#ifndef SELECTABLELIST_H
#define SELECTABLELIST_H
#include "LabelList.h"
#include "Selectable.h"
// a list where every label is selectable
/** ABSTRACT CLASS **/
class SelectableList : public LabelList, public Selectable {
   public:
    SelectableList(int x, int y, int width, int height) : LabelList(x, y, width, height) {}
    virtual void move(bool up);                  // this makes it abstract
    virtual void draw(WINDOW* window) override;  // override abstract function
   protected:
    int selectedLabel = 0;
};
#endif