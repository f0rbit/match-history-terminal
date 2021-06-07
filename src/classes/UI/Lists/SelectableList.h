#ifndef SELECTABLELIST_H
#define SELECTABLELIST_H
#include "../Selectable.h"
#include "LabelList.h"
// a list where every label is selectable
/** ABSTRACT CLASS **/
class SelectableList : public LabelList, public Selectable {
   public:
    SelectableList(int x, int y, int width, int height) : LabelList(x, y, width, height) {}
    virtual void move(bool up);    // this makes it abstract
    virtual void draw() override;  // override abstract function
   protected:
    int selectedLabel = 0;
};
#endif