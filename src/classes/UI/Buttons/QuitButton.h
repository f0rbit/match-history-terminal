#ifndef QUITBUTTON_H
#define QUITBUTTON_H
#include "Button.h"
// quit button exits the program.
class QuitButton : public Button {
   public:
    QuitButton(string text, int x, int y, int width, int height) : Button(text, x, y, width, height) {}
    virtual void execute() override;
};

#endif