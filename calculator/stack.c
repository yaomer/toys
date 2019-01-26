#include <stdio.h>
#include "cal.h"

#define MAXVAL 100

static int val[MAXVAL];    /*  值栈  */
static int sp = -1;    /*  栈顶指针  */

void
push(double c)
{
    if (!isfull())
        val[++sp] = c;
    else
        printf("stack is full\n");
}

double
pop(void)
{
    if (!isempty())
        return val[sp--];
    else {
        printf("stack is empty\n");
        return 0;
    }
}

double
top(void)
{
    if (!isempty())
        return val[sp];
    else {
        printf("stack is empty\n");
        return 0;
    }
}

int
isfull(void)
{
    return sp == MAXVAL - 1;
}

int
isempty(void)
{
    return sp == -1;
}
