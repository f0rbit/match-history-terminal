#ifndef UI_CLASSES_H
#define UI_CLASSES_H
#include <string.h>
#include <iostream>
#include <vector>
#include <ncurses.h>
using std::string;
using std::vector;




/** PURE ABSTRACT CLASS **/
class Element {
    public:
        Element() { id = currentID++; }; // makes a new unique id
        virtual void draw(WINDOW* window) = 0; // this makes it a pure abstract class, every class that inherits element must have draw() function
        int getX() { return this->x; };
        int getY() { return this->y; };

        void setX(int x) { this->x = x; };
        void setY(int y) { this->y = y; };

        int getID() { return this->id; };
    protected:
        int x;
        int y;
        int id;
        static int currentID; // stores how many element have been made in total
};

/** ABSTRACT CLASS **/
class Selectable {
    public:
        virtual void setFunction(void(*action)()) { this->function = action; }; // abstract function, can be overrriden.

        // returns if this element is currently selected
        bool isSelected(Selectable* e) {
            return (e->getSelectableID() == selectedID);              
        }
        int getSelectableID() { return this->selectableID; };
        void setSelectableID(int id) { this->selectableID = id; };
        static int selectedID; // the id of the currently selected element
    protected:
        void(*function)();  // function for when this selectable instance is selected and enter is pressed
        int selectableID = -1;
};

/** PURE ABSTRACT CLASS **/
class Inputable : public Selectable {
    public:
        void addCh(char c) { input += c; }; // adds cahracter to input string
        void delCh() { input = input.substr(0, input.size()-1); }; // removes the last character from input string
        string getInput() { return this->input; }; // returns input string
        void clearInput() { input = ""; };  // empties input string
        virtual void inputCharacter(int choice, char c) = 0; // this makes it a pure abstract class, every class that inherits Inputable requires a inputCharacter() function.
    protected:
        string input = "";
};


/** ABSTRACT CLASS **/
class Boxed : public Element { 
    public:
        Boxed(int width, int height) {
            setWidth(width);
            setHeight(height);
        }
        virtual void drawBox(WINDOW* window);
        void setWidth(int width) { this->width = width; };
        void setHeight(int height) { this->height = height; };
        int getWidth() { return this->width; };
        int getHeight() { return this->height; };
    protected:
        int width;
        int height;
};

// label with a box around it
class BoxedLabel : public Boxed {
    public:
        BoxedLabel(int width, int height) : Boxed(width, height) {};
        BoxedLabel(string text, int x, int y, int width, int height) : Boxed(width, height) {
            setX(x);
            setY(y);
            setText(text);
        }
        virtual void draw(WINDOW* window) override; // override the abstract function
        void setText(string text) { this->text = text; };
        string getText() { return this->text; };
    protected:
        string text;
};

// label without box around it
class Label : public Element {
    public:
        Label(string text, int x, int y) {
            setX(x);
            setY(y);
            setText(text);
        }

        virtual void draw(WINDOW* window) override; // override the abstract function
        void setText(string text) { this->text = text; };
        string getText() { return this->text; };
    protected:
        string text;
};

// a label that disappears when cooldown < 0
class DisappearingLabel : public Label {
    public: 
        DisappearingLabel(string text, int x, int y) : Label(text,x,y) {
            cooldown = 10; // set the initial cooldown to 10
        }
        virtual void draw(WINDOW* window) override; // override abstract function
        void tick() { cooldown--; }; // reduce cooldown by 1
    protected:
        int cooldown;
};

// a label that has multiple lines.
// isn't classified as an element as is only used within
// MutliLineLabelList class
class MultiLineLabel {
    public:
        MultiLineLabel(vector<string> list) {
            this->strings = list;
        }
        vector<string> getStrings() { return this->strings; };
        void addString(string str) { strings.push_back(str); };
        void draw(WINDOW* window, int x, int y);
        string getHiddenValue() { return this->hiddenValue; };
        void setHiddenValue(string value) { this->hiddenValue = value; };
    protected:
        vector<string> strings; // the strings to be drawn
        string hiddenValue; // hidden value can store data.

};

/** PURE ABSTRACT CLASS **/
class Button : public BoxedLabel, public Selectable {
    public:
        Button(string text, int x, int y, int width, int height) : BoxedLabel(text, x, y, width, height) { }
        virtual void draw(WINDOW* window) override; // override abstract function
        virtual void execute() = 0; // this makes it a pure abstract class, every member that inherits Button must have an execute() function
};


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

// list of strings
class LabelList : public Boxed {
    public:
        LabelList(int x, int y, int width, int height) : Boxed(width, height) {
            setX(x);
            setY(y);
        }
        void addLabel(string str) { this->labels.push_back(str); }; // adds a string
        vector<string> getLabels() { return labels; }; // returns every string
        virtual void draw(WINDOW* window) override;
        string getTitle() { return this->title; };
        void setTitle(string title) { this->title = title; };
        void clear() { this->labels.clear(); }; // clears the strings
    protected:
        vector<string> labels; // stores all the strings
        string title; // optional title to be drawn above the list.
};

// a list where every label is selectable
/** ABSTRACT CLASS **/
class SelectableList : public LabelList, public Selectable {
    public:
        SelectableList(int x, int y, int width, int height) : LabelList(x,y,width,height) {}
        virtual void move(bool up); // this makes it abstract
        virtual void draw(WINDOW* window) override; // override abstract function
    protected:
        int selectedLabel = 0;
};

// a list of MutliLineLabels
class MultiLineLabelList : public SelectableList {
    public:
        MultiLineLabelList(int x, int y, int width, int height) : SelectableList(x,y,width, height) {}
        void addLabel(MultiLineLabel* label) { this->labels.push_back(label); }; // adds label to vector
        vector<MultiLineLabel*> getLabels() { return this->labels; };
        string getTitle() { return this->title; };
        void setTitle(string title) { this->title = title; };
        virtual void draw(WINDOW* window) override; // override abstract function
        void move(bool up); // moves up or down the list
        void clear() { labels.clear(); }; // clears the labels vector
        int getSelectedLabel() { return this->selectedLabel; };// gets the index of the currently selected label
    protected:
        vector<MultiLineLabel*> labels; // stores all the labels
        string title; // optional title, drawn above the list
        int selectedLabel = 0; // the currently selected label
};

// list that is scrollable.
class ScrollableList : public SelectableList {
    public:
        ScrollableList(int x, int y, int width, int height) : SelectableList(x,y,width,height) {}
        virtual void draw(WINDOW* window) override;
        virtual void move(bool up) override;
    protected:
        int yOffset = 0;    
};

/** SIZEABLE CLASS **/
// stores information about each screen
// each screen contains a list of elements
// and a seperate list of selectable elements
// the lists oftentimes share the same objects
class Screen {
    public:
        Screen();
        Element **getElements() { 
            return this->elements;
        };
        Selectable **getSelectables() {
            return this->selectableInstances;
        }
        // adds an element to this screen
        void addElement(Element* e) {
            Element** new_array = new Element*[elementCount+1];
            for (int i = 0; i < elementCount; i++) {
                new_array[i] =  elements[i];
            }
            new_array[elementCount] = e;
            elementCount++;
            elements = new_array;

            // if this object is of a selectable nature, add it to the seperate selectable list
            if (Selectable* selectable = dynamic_cast<Selectable*>(e)) {
                // add selectable to array
                addSelectable(selectable);
            }
        } 
        // adds a selectable element to the selectables array.
        void addSelectable(Selectable* e) {
            Selectable** new_array = new Selectable*[selectableCount+1];
            for (int i = 0; i < selectableCount; i++) {
                new_array[i] =  selectableInstances[i];
            }
            new_array[selectableCount] = e;
            e->setSelectableID(selectableCount);
            selectableCount++;
            selectableInstances = new_array;
        }
        int getScreenID() { return this->screenID; };
        int getElementCount() { return this->elementCount; };
        int getSelectableCount() { return this->selectableCount; };

        // returns the currently selected element.
        Selectable* getSelected() {
            return getSelectables()[Selectable::selectedID];
        }

    protected:
        Element** elements;
        Selectable** selectableInstances;
        int selectableCount = -1;
        int elementCount = 1;
        static int screenCount; // keeps track of how many screens have been made
        int screenID; // id of this screen

        // cleanup
        ~Screen() {
            delete[] elements, selectableInstances;
        }
};



// back button goes to last screen
class BackButton : public Button {
    public:
        BackButton(string text, int x, int y, int width, int height, int scrID) : Button(text,x,y,width,height) {
             setScreenID(scrID);
        }
        void setScreenID(int id) { this->screenID = id; };
        int getScreenID() { return this->screenID; };
        virtual void execute() override;
    protected:
        int screenID;
};

// enter button after summoner name is entered
class EnterButton : public Button {
    public:
        EnterButton(string text, int x, int y, int width, int height) : Button(text,x,y,width,height) {}
        void setBox(InputBox* box) { this->box = box; };
        InputBox* getBox() { return this->box; };
        virtual void execute() override;
    protected:
        InputBox* box;
        ~EnterButton() {
            delete[] box;
        }
};

// quit button exits the program.
class QuitButton : public Button {
    public:
        QuitButton(string text, int x, int y, int width, int height) : Button(text,x,y,width,height) {}
        virtual void execute() override;

};

#endif