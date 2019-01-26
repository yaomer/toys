#ifndef _SNAKE_H
#define _SNAKE_H

#define DELAY  120    /*  设置延时  */                                                                                                            
/*  蛇的活动地图的大小  */
#define ROW    (LINES - 3)
#define COL    (COLS - 24) 
 
typedef struct snake {     /*  蛇身节点  */
    int sx;       /*  行坐标  */
    int sy;       /*  列坐标  */
    struct snake *next;
} Snake;

extern int score;

void    init(void);
void    get_keyboard(void);
void    set_color(void);
void    draw_initscr(void);
void    draw_map(void);
void    creat_snake(void);
void    creat_food(void);
void    display_score(void);
void    display_snake(int);
void    add_snake(void);
void    del_snake(void);
int     is_eat_food(void);
int     is_crash(void);
void    lock_snake(void);
void    draw_overscr(void);
int     set_ticker(int);
void    over(void);

#endif  /* _SNAKE_H */
