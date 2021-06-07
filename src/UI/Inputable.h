#ifndef INPUTABLE_H
#define INPUTABLE_H

#include <string>

#include "Selectable.h"

/** PURE ABSTRACT CLASS **/
class Inputable : public Selectable {
   public:
    void addCh(char c) { input += c; };                           // adds cahracter to input string
    void delCh() { input = input.substr(0, input.size() - 1); };  // removes the last character from input string
    std::string getInput() { return this->input; };               // returns input string
    void clearInput() { input = ""; };                            // empties input string
    virtual void inputCharacter(int choice, char c) = 0;          // this makes it a pure abstract class, every class that inherits Inputable requires a inputCharacter() function.
   protected:
    std::string input = "";
};

#endif