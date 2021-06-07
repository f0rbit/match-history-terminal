#include "InputManager.h"

#include "../UI/Lists/SelectableList.h"
#include "../UI/Screen.h"

InputManager* InputManager::instance = new InputManager();

// gets the input from the user
int InputManager::input() {
    int choice = wgetch(inputWindow);  // timesout after 100ms
    switch (choice) {
        case KEY_UP:  // arrow keys
        case KEY_DOWN:
            if (SelectableList* list = dynamic_cast<SelectableList*>(getCurrentScreen()->getSelected())) {
                list->move(choice == KEY_UP);
            }
            break;
        case '\t':  // tab
            if (Selectable::selectedID < getCurrentScreen()->getSelectableCount() - 1) {
                Selectable::selectedID++;
            } else {
                Selectable::selectedID = 0;
            }
            return -9;
        case 8:
        case KEY_BACKSPACE:  // backspace
            return -10;
        case 353:  // shift-tab
            if (Selectable::selectedID > 0) {
                Selectable::selectedID--;
            } else {
                Selectable::selectedID = getCurrentScreen()->getSelectableCount() - 1;
            }
            return -8;
        case 10:  // enter keys
        case KEY_ENTER:
            return -7;
        default:
            break;
    }
    return choice;
}