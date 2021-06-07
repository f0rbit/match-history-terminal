#ifndef ELEMENT_H
#define ELEMENT_H
#include <ncurses.h>
/** PURE ABSTRACT CLASS **/
class Element {
   public:
    Element() { id = currentID++; };  // makes a new unique id
    virtual void draw() = 0;          // this makes it a pure abstract class, every class that inherits element must have draw() function
    int getX() { return this->x; };
    int getY() { return this->y; };

    void setX(int x) { this->x = x; };
    void setY(int y) { this->y = y; };

    int getID() { return this->id; };

   protected:
    int x;
    int y;
    int id;
    static int currentID;  // stores how many element have been made in total
};
#endif