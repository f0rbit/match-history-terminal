#ifndef SELECTABLE_H
#define SELECTABLE_H
/** ABSTRACT CLASS **/
class Selectable {
   public:
    virtual void setFunction(void (*action)()) { this->function = action; };  // abstract function, can be overrriden.

    // returns if this element is currently selected
    bool isSelected(Selectable* e) {
        return (e->getSelectableID() == selectedID);
    }
    int getSelectableID() { return this->selectableID; };
    void setSelectableID(int id) { this->selectableID = id; };
    static int selectedID;  // the id of the currently selected element
   protected:
    void (*function)();  // function for when this selectable instance is selected and enter is pressed
    int selectableID = -1;
};
#endif