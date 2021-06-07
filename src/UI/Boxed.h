#ifndef BOXED_H
#define BOXED_H

#include "Element.h"

/** ABSTRACT CLASS **/
class Boxed : public Element {
   public:
    Boxed(int width, int height) {
        setWidth(width);
        setHeight(height);
    }
    virtual void drawBox(WINDOW* window);
    void setWidth(int width) { this->width = width; };
    void setHeight(int height) { this->height = height; };
    int getWidth() { return this->width; };
    int getHeight() { return this->height; };

   protected:
    int width;
    int height;
};

#endif