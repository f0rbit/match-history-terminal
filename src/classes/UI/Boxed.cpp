#include "Boxed.h"

void Boxed::drawRectangle(int x, int y, int width, int height) {
    // fills the rectangle with spaces
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            mvaddch(y + j, x + i, ' ');
        }
    }
    // draws box around
    mvhline(y, x, 0, x + width - x);
    mvhline(y + height, x, 0, x + width - x);
    mvvline(y, x, 0, y + height - y);
    mvvline(y, x + width, 0, y + height - y);
    mvaddch(y, x, ACS_ULCORNER);
    mvaddch(y + height, x, ACS_LLCORNER);
    mvaddch(y, x + width, ACS_URCORNER);
    mvaddch(y + height, x + width, ACS_LRCORNER);
}

void Boxed::drawBox() {
    // draws a box
    drawRectangle(getX(), getY(), getWidth(), getHeight());
}