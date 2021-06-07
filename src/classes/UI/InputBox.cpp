#include "InputBox.h"

#include <string>
void InputBox::draw() {
    // draws an input box
    // will have white highlight if selelected
    if (isSelected(this)) {
        attron(A_STANDOUT);
    }
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    mvaddch(getY() + 1, getX() + 1, ('>'));
    mvaddstr(getY() + 1, getX() + 3, getInput().c_str());
    attroff(A_STANDOUT);
}

void InputBox::inputCharacter(int choice, char c) {
    std::string acceptedCharacters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 ";  // all the characters that are accepted by InputBox
    if (choice == -9 || choice == -8 || choice == -7 || choice == -1) return;                            // if it's enter, nothing, or an arrow key.
    if (isSelected(this)) {                                                                              // only works if this InputBox is selected.
        if (choice == -10) {                                                                             // backspace
            delCh();                                                                                     // delete the last character
        } else {
            if (getInput().length() < width - 3) {
                if (acceptedCharacters.find(c) == std::string::npos) return;  // checks to see if there is an instance of (char) c inside (string) acceptedCharacters, returns out if not.
                addCh(c);                                                     // add the character to the inputbox.text
            }
        }
    }
}