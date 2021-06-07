#ifndef BACKBUTTON_H
#define BACKBUTTON_H
#include "Button.h"
// back button goes to last screen
class BackButton : public Button {
   public:
    BackButton(std::string text, int x, int y, int width, int height, int scrID) : Button(text, x, y, width, height) {
        setScreenID(scrID);
    }
    void setScreenID(int id) { this->screenID = id; };
    int getScreenID() { return this->screenID; };
    virtual void execute() override;

   protected:
    int screenID;
};
#endif