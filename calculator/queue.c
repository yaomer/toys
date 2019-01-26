#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "cal.h"

Expr *
init_expr(void)
{
    Expr *p;

    assert(p = malloc(sizeof(Expr)));
    p->front = p->rear = NULL;

    return p;
}

void
en_expr(Expr *Q, int new_val)
{
    _Expr *p;

    assert(p = malloc(sizeof(_Expr)));
    p->val = new_val;

    if (is_expr_empty(Q))   /* 初始化队列 */
        Q->front = Q->rear = p;
    else {   /* 插到队尾 */
        Q->rear->next = p;
        Q->rear = p;
    }
    p->next = NULL;
}

void
de_expr(Expr *Q)
{
    _Expr *tmp;

    if (!is_expr_empty(Q)) {
        tmp = Q->front->next;
        free(Q->front);
        if (!(Q->front = tmp))   /* 队列为空 */
            Q->rear = NULL;
    } else
        printf("queue is empty\n");
}

int
head_expr(Expr *Q)
{
    if (!is_expr_empty(Q))
        return Q->front->val;
    else {
        printf("queue is empty\n");
        return 0;
    }
}

int
is_expr_full(Expr *Q)
{
    return 0;
}

int
is_expr_empty(Expr *Q)
{
    assert(Q);
    return Q->front == NULL;
}

void
clear_expr(Expr **Q)
{
    assert(Q);
    while (!is_expr_empty(*Q))
        de_expr(*Q);
    free(*Q);
    *Q = NULL;
}
