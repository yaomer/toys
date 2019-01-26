#include <curses.h>
#include "snake.h"

void
draw_initscr()
{
    int i;

    attron(COLOR_PAIR(4));
    for (i = COLS / 2 - 20; i <= COLS / 2 + 20; i += 2) {
        mvaddch(LINES / 2 - 4, i, '@');
        mvaddch(LINES / 2 + 6, i, '@');
    }
    for (i = LINES / 2 - 3; i <= LINES / 2 + 5; i++) {
        mvaddch(i, COLS / 2 - 22, '@');
        mvaddch(i, COLS / 2 + 22, '@');
    }
    mvaddstr(LINES / 2 + 8, COLS / 2 - 13, "Press any key to continue...");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(5));
    mvaddstr(LINES / 2 - 2, COLS / 2 - 8, "Welcome to snake");
    mvaddstr(LINES / 2 - 1, COLS / 2 - 10, "emmmmmmmmm, however,");
    mvaddstr(LINES / 2, COLS / 2 - 20, "now that you are here, you must be greedy!");
    mvaddstr(LINES / 2 + 1, COLS / 2 - 11, "~~o~~o~~o~~o~~o~~o~~o~~");
    mvaddstr(LINES / 2 + 2, COLS / 2 - 5, "Game Rules:");
    mvaddstr(LINES / 2 + 3, COLS / 2 - 18, "1. use direciton keys to move snake");
    mvaddstr(LINES / 2 + 4, COLS / 2 - 18, "2. snake is green space, food is red");
    getch();
    attroff(COLOR_PAIR(5));
    refresh();
    clear();   /*  清屏  */
}

void
draw_map(void)
{
    int i;

    attron(COLOR_PAIR(3));
    for (i = 3; i < COLS - 2; i += 2) {
        mvaddch(2, i, ' ');
        mvaddch(LINES - 2, i, ' ');
    }
    for (i = 3; i < LINES - 1; i += 2) {
        mvaddch(i, 3, ' ');
        mvaddch(i, COL + 1, ' ');
        mvaddch(i, COLS - 4, ' ');
    }
    attroff(COLOR_PAIR(3));
    attron(COLOR_PAIR(4));
    mvaddstr(10, 104, "Your Score:");
    mvaddstr(12, 102, "^0^ smiling ^0^");
    attroff(COLOR_PAIR(4));
}

void
draw_overscr(void)
{
    int i;

    clear();
    attron(COLOR_PAIR(4));
    for (i = COLS / 2 - 16; i <= COLS / 2 + 16; i += 2) {
        mvaddch(LINES / 2 - 4, i, '@');
        mvaddch(LINES / 2 + 3, i, '@');
    }
    for (i = LINES / 2 - 3; i <= LINES / 2 + 2; i++) {
        mvaddch(i, COLS / 2 - 18, '@');
        mvaddch(i, COLS / 2 + 18, '@');
    }
    mvaddstr(LINES / 2 + 5, COLS / 2 - 13, "Press any key to continue...");
    attroff(COLOR_PAIR(4));
    attron(COLOR_PAIR(5));
    mvaddstr(LINES / 2 - 1, COLS / 2 - 13, "I'm sorry, the game is over");
    mvaddstr(LINES / 2, COLS / 2 - 9, "But expect you back");
    attroff(COLOR_PAIR(5));
    refresh();
    getch();
}

void
display_score(void)
{
    attron(COLOR_PAIR(5));
    move(11, 108);
    printw("%d", score++);
    attroff(COLOR_PAIR(5));
}
