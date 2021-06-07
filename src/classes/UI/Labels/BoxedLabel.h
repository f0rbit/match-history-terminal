#ifndef BOXEDLABEL_H
#define BOXEDLABEL_H
#include <string>

#include "../Boxed.h"

// label with a box around it
class BoxedLabel : public Boxed {
   public:
    BoxedLabel(int width, int height) : Boxed(width, height){};
    BoxedLabel(std::string text, int x, int y, int width, int height) : Boxed(width, height) {
        setX(x);
        setY(y);
        setText(text);
    }
    virtual void draw() override;  // override the abstract function
    void setText(std::string text) { this->text = text; };
    std::string getText() { return this->text; };

   protected:
    std::string text;
};

#endif
