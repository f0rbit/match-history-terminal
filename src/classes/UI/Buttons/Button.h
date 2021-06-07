#ifndef BUTTON_H
#define BUTTON_H
#include <string>

#include "../Labels/BoxedLabel.h"
/** PURE ABSTRACT CLASS **/
class Button : public BoxedLabel, public Selectable {
   public:
    Button(std::string text, int x, int y, int width, int height) : BoxedLabel(text, x, y, width, height) {}
    virtual void draw() override;  // override abstract function
    virtual void execute() = 0;    // this makes it a pure abstract class, every member that inherits Button must have an execute() function
};

#endif