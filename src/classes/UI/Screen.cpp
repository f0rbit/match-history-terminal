#include "Screen.h"

int Screen::screenCount = 0;

Screen::Screen() {
    // screen constructor
    elements = (new Element*[0]);
    elementCount = 0;
    selectableInstances = (new Selectable*[0]);
    selectableCount = 0;
}

Screen::~Screen() {
    for (int i = 0; i < getElementCount(); i++) {
        delete[] getElements()[i];
    }
    for (int i = 0; i < getSelectableCount(); i++) {
        delete[] getSelectables()[i];
    }
    delete[] elements, selectableInstances;
}

// returns the currently selected element.
Selectable* Screen::getSelected() {
    return getSelectables()[Selectable::selectedID];
}

// adds a selectable element to the selectables array.
void Screen::addSelectable(Selectable* e) {
    Selectable** new_array = new Selectable*[selectableCount + 1];
    for (int i = 0; i < selectableCount; i++) {
        new_array[i] = selectableInstances[i];
    }
    new_array[selectableCount] = e;
    e->setSelectableID(selectableCount);
    selectableCount++;
    selectableInstances = new_array;
}

// adds an element to this screen
void Screen::addElement(Element* e) {
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