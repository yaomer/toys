#ifndef CAL_H
#define CAL_H

#define MAXOP  20           /*  运算符或操作数的最大长度  */
#define NUMBER '0'          /*  标识找到一个数  */

typedef struct expr {
    int val;
    struct expr *next;
} _Expr;

typedef struct {
    _Expr *front;
    _Expr *rear;
} Expr;

Expr    *init_expr(void);
void    en_expr(Expr *, int);
void    de_expr(Expr *);
int     head_expr(Expr *);
void    clear_expr(Expr **);
int     is_expr_full(Expr *);
int     is_expr_empty(Expr *);

int     getop(char *);
void    exp_trans(void);
int     op_prior(int);
double  fac(double);

void    push(double);
double  pop(void);
double  top(void);
int     isfull(void);
int     isempty(void);

#endif    /*  CAL_H  */
