#include <stdlib.h>
#include <signal.h>
#include <curses.h>
#include "snake.h"

int
main(void)
{
    init();
    signal(SIGALRM, display_snake);
    get_keyboard();
    endwin();
    return 0;
}

/*
 * 游戏初始化
 * 包括curses库和游戏界面的初始化
 */
void
init(void)
{
    initscr();   /*  初始化curses  */
    start_color();  /*  初始化颜色表  */
    set_color();    /*  设置颜色  */
    box(stdscr, ACS_VLINE, ACS_HLINE);   /*  绘制一个同物理终端大小相同的窗口  */
    noecho();    /*  关闭键入字符的回显  */
    cbreak();    /*  字符一键入，直接传递给程序  */
    curs_set(0);    /*  隐藏光标  */
    keypad(stdscr, true);    /*  开启逻辑键  */
    draw_initscr();
    draw_map();
    creat_snake();
    creat_food();
    display_score();
    refresh();
}

/*
 * 刷新蛇身
 */
void
display_snake(int signo)
{
    if (is_crash())
        over();
    if (is_eat_food()) {
        add_snake();
        creat_food();
        display_score();
    } else {
        add_snake();
        del_snake();
    }
    refresh();
}

void
over(void)
{
    draw_overscr();
    endwin();    /*  结束调用curses  */
    exit(0);
}
