#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include "cal.h"

int
main(void)
{
    char s[MAXOP];    /*  存储获取的运算符或操作数  */
    int type;
    double op2;

    while ((type = getop(s)) != EOF)
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (fabs(op2) > DBL_EPSILON)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if (fabs(op2) > DBL_EPSILON)
                push(fmod(pop(), op2));
            else
                printf("error: zero divisor\n");
            break;
        case '^':
            op2 = pop();
            push(pow(pop(), op2));
            break;
        case '!':
            push(fac(pop()));
            break;
        case '\n':
            printf("\t%.8g\n", pop());
            break;
        default:
            printf("error: unknow command %s\n", s);
            break;
        }
    return 0;
}
