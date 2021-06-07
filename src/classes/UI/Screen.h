#ifndef SCREEN_H
#define SCREEN_H
#include "Element.h"
#include "Selectable.h"
/** SIZEABLE CLASS **/
// stores information about each screen
// each screen contains a list of elements
// and a seperate list of selectable elements
// the lists oftentimes share the same objects
class Screen {
   public:
    Screen();
    Element** getElements() { return this->elements; };
    Selectable** getSelectables() { return this->selectableInstances; }
    // adds an element to this screen
    void addElement(Element* e);
    // adds a selectable element to the selectables array.
    void addSelectable(Selectable* e);

    int getScreenID() { return this->screenID; };
    int getElementCount() { return this->elementCount; };
    int getSelectableCount() { return this->selectableCount; };

    // returns the currently selected element.
    Selectable* getSelected();

    ~Screen();

   protected:
    Element** elements;
    Selectable** selectableInstances;
    int selectableCount = -1;
    int elementCount = 1;
    static int screenCount;  // keeps track of how many screens have been made
    int screenID;            // id of this screen
};

#endif