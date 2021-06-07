#include "LabelList.h"
#include "MultiLineLabelList.h"
#include "ScrollableList.h"
#include "SelectableList.h"

void LabelList::draw() {
    // draws a list of strings.
    mvaddstr(getY() - 1, getX() + 1, getTitle().c_str());
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    for (int i = 0; i < getLabels().size(); i++) {
        mvaddstr(getY() + i + 1, getX() + 1, getLabels().at(i).c_str());
    }
}

void MultiLineLabelList::draw() {
    // draws a list of lists of strings with lines inbetween.
    mvaddstr(getY() - 1, getX() + 1, getTitle().c_str());
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    int currentY = getY() + 1;  // keep track of y position
    for (int i = 0; i < getLabels().size(); i++) {
        if (i > 0) {
            mvhline(currentY++, x + 1, 0, getWidth() - 1);
        }
        if (isSelected(this) && selectedLabel == i) {
            attron(A_STANDOUT);
        }
        getLabels().at(i)->draw(getX() + 1, currentY);
        currentY += getLabels().at(i)->getStrings().size();
        attroff(A_STANDOUT);
    }
}

void MultiLineLabelList::move(bool up) {
    if (up) {
        if (selectedLabel > 0) selectedLabel--;
    } else {
        if (selectedLabel < labels.size() - 1) selectedLabel++;
    }
}

void SelectableList::draw() {
    mvaddstr(getY() - 1, getX() + 1, getTitle().c_str());
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    for (int i = 0; i < getLabels().size(); i++) {
        if (isSelected(this) && selectedLabel == i) {
            attron(A_STANDOUT);
        }
        mvaddstr(getY() + i + 1, getX() + 1, getLabels().at(i).c_str());
        attroff(A_STANDOUT);
    }
}

void SelectableList::move(bool up) {
    if (up) {
        if (selectedLabel > 0) selectedLabel--;
    } else {
        if (selectedLabel < labels.size() - 1) selectedLabel++;
    }
}

void ScrollableList::draw() {
    mvaddstr(getY() - 1, getX() + 1, getTitle().c_str());
    drawRectangle(getX(), getY(), getWidth(), getHeight());
    for (int i = yOffset; i < getHeight() - 1 + yOffset; i++) {
        if (isSelected(this) && selectedLabel == i) {
            attron(A_STANDOUT);
        }
        mvaddstr(getY() + i + 1 - yOffset, getX() + 1, getLabels().at(i).c_str());
        attroff(A_STANDOUT);
    }

    // draw arrows if more elements are present off screen
    if (yOffset < getHeight() - 3) {
        mvaddch(getY() + getHeight() - 1, getX() + getWidth() - 1, 'v');
    }
    if (yOffset > 0) {
        mvaddch(getY() + 1, getX() + getWidth() - 1, '^');
    }
}

void ScrollableList::move(bool up) {
    if (up) {
        if (selectedLabel > 0) {
            selectedLabel--;

            if (selectedLabel < yOffset) {
                yOffset--;
            }
        }
    } else {
        if (selectedLabel < labels.size() - 1) {
            selectedLabel++;

            if (selectedLabel > getHeight() - 2) {
                yOffset++;
            }
        }
    }
}