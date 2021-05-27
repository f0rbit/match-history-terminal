#include <ncurses.h>

int main() {
    initscr();

    int row = 5;
    int col = 0;
    curs_set(0);

    for(char c = 65; c <= 90; c++)
    {
        move(row++, col++);
        addch(c);
        refresh();
        napms(100);
    }

    row = 5;
    col = 3;

    for(char c = 97; c <= 122; c++)
    {
        mvaddch(row++, col++, c);
        refresh();
        napms(100);
    }

    curs_set(1);

    addch('\n');

    refresh();
    getch();
    endwin();
    return 0;
}