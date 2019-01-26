#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <curses.h>
#include "snake.h"

Snake *head, *tail;   /*  蛇头、蛇尾  */
int dx, dy;           /*  一组方向坐标  */
int score;            /*  得分  */

/*
 * 显示一个蛇身节点
 */
void
creat_snake(void)
{
    assert(head = tail = malloc(sizeof(Snake)));
    head->next = NULL;
    srand(clock());   /*  以当前挂钟时间作随机种子数  */
    while ((head->sx = rand() % (ROW - 3) + 3) % 2 == 0)
        ;
    while ((head->sy = rand() % (COL - 4) + 4) % 2 != 0)
        ;
    attron(COLOR_PAIR(1));
    mvaddch(head->sx, head->sy, ' ');
    attroff(COLOR_PAIR(1));
}

/*
 * 显示一个食物
 */
void
creat_food(void)
{
    int i, j;

    srand(clock());
    while ((i = rand() % (ROW - 3) + 3) % 2 == 0)
        ;
    while ((j = rand() % (COL - 4) + 4) % 2 != 0)
        ;
    move(i, j);
    if (inch() == 288)    /*  食物不能覆盖蛇身  */
        creat_food();
    attron(COLOR_PAIR(2));
    addch(' ');
    attroff(COLOR_PAIR(2));
}

/*
 * 增长蛇身（在蛇头增加两个节点）
 */
void
add_snake(void)
{
    Snake *p, *q;

    assert(p = malloc(sizeof(Snake)));
    assert(q = malloc(sizeof(Snake)));
    head->next = p;
    p->next = q;
    q->next = NULL;
    attron(COLOR_PAIR(1));
    p->sx = head->sx + dx;
    p->sy = head->sy + dy;
    mvaddch(p->sx, p->sy, ' ');
    q->sx = p->sx + dx;
    q->sy = p->sy + dy;
    mvaddch(q->sx, q->sy, ' ');
    attroff(COLOR_PAIR(1));
    head = q;
}

/*
 * 缩减蛇身（在蛇尾删除两个节点）
 */
void
del_snake(void)
{
    Snake *tmp;

    assert(tail);
    mvaddch(tail->sx, tail->sy, ' ');
    mvaddch(tail->next->sx, tail->next->sy, ' ');
    tmp = tail->next->next;
    free(tail->next);
    free(tail);
    tail = tmp;
}

/*
 * 判断蛇是否吃到食物
 */
int
is_eat_food(void)
{
    return inch() == 544;
}

/*
 * 判断蛇是否撞到障碍物
 */
int
is_crash(void)
{
    move(head->sx + 2 * dx, head->sy + 2 * dy);
    if (head->sx <= 2 || head->sx >= ROW + 1||
        head->sy <= 3 || head->sy >= COL + 1)   /*  撞到边界  */
        return 1;
    return inch() == 288;   /*  撞到自身  */
}

/*
 * 监听键盘
 * 忽略除方向键之外的其他键，按'q'可退出游戏
 */
void
get_keyboard(void)
{
    int c;

next:
    while ((c = getch()) != 'q') {
        switch(c) {
        case KEY_UP:
            dx = -1; dy = 0;
            break;
        case KEY_DOWN:
            dx = 1; dy = 0;
            break;
        case KEY_LEFT:
            dx = 0; dy = -1;
            break;
        case KEY_RIGHT:
            dx = 0; dy = 1;
            break;
        default:
            goto next;
        }
        lock_snake();
        set_ticker(DELAY);
    }
}

/*
 * 防止蛇身反向移动
 */
void
lock_snake(void)
{
    static int lx, ly;

    if (dx && dx + lx == 0)
        dx = lx;
    if (dy && dy + ly == 0)
        dy = ly;
    lx = dx;
    ly = dy;
}
