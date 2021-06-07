#ifndef BOXEDLABEL_H
#define BOXEDLABEL_H
#include <string>

#include "Boxed.h"

// label with a box around it
class BoxedLabel : public Boxed {
   public:
    BoxedLabel(int width, int height) : Boxed(width, height){};
    BoxedLabel(string text, int x, int y, int width, int height) : Boxed(width, height) {
        setX(x);
        setY(y);
        setText(text);
    }
    virtual void draw(WINDOW* window) override;  // override the abstract function
    void setText(string text) { this->text = text; };
    string getText() { return this->text; };

   protected:
    std::string text;
};

#endif
