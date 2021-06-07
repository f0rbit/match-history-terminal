#ifndef UI_CLASSES_H
#define UI_CLASSES_H
#include <ncurses.h>
#include <string.h>

#include <iostream>
#include <vector>
using std::string;
using std::vector;

/** SIZEABLE CLASS **/
// stores information about each screen
// each screen contains a list of elements
// and a seperate list of selectable elements
// the lists oftentimes share the same objects
class Screen {
   public:
    Screen();
    Element** getElements() {
        return this->elements;
    };
    Selectable** getSelectables() {
        return this->selectableInstances;
    }
    // adds an element to this screen
    void addElement(Element* e) {
        Element** new_array = new Element*[elementCount + 1];
        for (int i = 0; i < elementCount; i++) {
            new_array[i] = elements[i];
        }
        new_array[elementCount] = e;
        elementCount++;
        elements = new_array;

        // if this object is of a selectable nature, add it to the seperate selectable list
        if (Selectable* selectable = dynamic_cast<Selectable*>(e)) {
            // add selectable to array
            addSelectable(selectable);
        }
    }
    // adds a selectable element to the selectables array.
    void addSelectable(Selectable* e) {
        Selectable** new_array = new Selectable*[selectableCount + 1];
        for (int i = 0; i < selectableCount; i++) {
            new_array[i] = selectableInstances[i];
        }
        new_array[selectableCount] = e;
        e->setSelectableID(selectableCount);
        selectableCount++;
        selectableInstances = new_array;
    }
    int getScreenID() { return this->screenID; };
    int getElementCount() { return this->elementCount; };
    int getSelectableCount() { return this->selectableCount; };

    // returns the currently selected element.
    Selectable* getSelected() {
        return getSelectables()[Selectable::selectedID];
    }

   protected:
    Element** elements;
    Selectable** selectableInstances;
    int selectableCount = -1;
    int elementCount = 1;
    static int screenCount;  // keeps track of how many screens have been made
    int screenID;            // id of this screen

    // cleanup
    ~Screen() {
        delete[] elements, selectableInstances;
    }
};

#endif