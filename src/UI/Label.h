#ifndef LABEL_H
#define LABEL_H
#include <string>

#include "Element.h"
// label without box around it
class Label : public Element {
   public:
    Label(std::string text, int x, int y) {
        setX(x);
        setY(y);
        setText(text);
    }

    virtual void draw(WINDOW* window) override;  // override the abstract function
    void setText(std::string text) { this->text = text; };
    std::string getText() { return this->text; };

   protected:
    std::string text;
};

#endif