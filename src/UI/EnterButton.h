#ifndef ENTERBUTTON_H
#define ENTERBUTTON_H
#include "Button.h"
#include "InputBox.h"
// enter button after summoner name is entered
class EnterButton : public Button {
   public:
    EnterButton(string text, int x, int y, int width, int height) : Button(text, x, y, width, height) {}
    void setBox(InputBox* box) { this->box = box; };
    InputBox* getBox() { return this->box; };
    virtual void execute() override;

   protected:
    InputBox* box;
    ~EnterButton() {
        delete[] box;
    }
};

#endif