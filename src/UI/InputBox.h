#ifndef INPUTBOX_H
#define INPUTBOX_H
#include "Boxed.h"
#include "Inputable.h"
// input function
class InputBox : public Boxed, public Inputable {
   public:
    InputBox(int x, int y, int width, int height) : Boxed(width, height) {
        setX(x);
        setY(y);
    };
    virtual void draw(WINDOW* window) override;
    virtual void inputCharacter(int choice, char c) override;
};

#endif