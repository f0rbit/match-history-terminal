#ifndef DISAPPEARINGLABEL_H
#define DISAPPEARINGLABEL_H

#include "Label.h"

// a label that disappears when cooldown < 0
class DisappearingLabel : public Label {
   public:
    DisappearingLabel(std::string text, int x, int y) : Label(text, x, y) {
        cooldown = 10;  // set the initial cooldown to 10
    }
    virtual void draw(WINDOW* window) override;  // override abstract function
    void tick() { cooldown--; };                 // reduce cooldown by 1
   protected:
    int cooldown;
};

#endif