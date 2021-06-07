#include "BoxedLabel.h"
#include "DisappearingLabel.h"
#include "Label.h"
#include "MultiLineLabel.h"

void Label::draw() {
    // draws the label
    mvaddstr(getY() + 1, getX() + 1, getText().c_str());
}

void MultiLineLabel::draw(int x, int y) {
    // draws a list of strings
    for (int i = 0; i < getStrings().size(); i++) {
        mvaddstr(y + i, x, getStrings().at(i).c_str());
    }
}

void BoxedLabel::draw() {
    // draws a label with a box around it
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    mvaddstr(getY() + 1, getX() + 1, getText().c_str());
}

void DisappearingLabel::draw() {
    if (cooldown > 0) {
        mvaddstr(getY() + 1, getX() + 1, getText().c_str());
    } else {
        mvhline(getY() + 1, getX() + 1, ' ', getText().length());
    }
}