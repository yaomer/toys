#include <stdio.h>
#include <ctype.h>
#include "cal.h"

Expr *expr;

/*
 *  遇见数字，输出；遇见操作符，入栈；遇见比栈顶操作符优先级更低的，弹出栈中比遇见的操作符优先级高的；
 *  遇见')'，弹出栈中'('之后的所有操作符
 */

/*
 * 将输入中的后缀表达式转换为中缀表达式
 */
void
exp_trans(void)
{
    int c, lastc = 'a';

    while ((c = getchar()) != EOF) {
        if (isspace(c) || isdigit(c) || c == '.') {        /*  原样保存空白符和数字  */
            if (c == '\n') {
                while (!isempty()) {        /*  遇见\n，弹出栈中所有操作符  */
                    en_expr(expr, pop());
                    en_expr(expr, ' ');
                }
                break;
            }
            if (!isspace(lastc) || !isspace(c))    /* 遇见连续的多个空白符只保存一个 */
                en_expr(expr, c);
        } else if (op_prior(c) || c == ')') {
            if (isempty() && c != '-') {
                push(c);
            } else if (c == ')') {
                while (!isempty()) {
                    if (top() == '(') {
                        pop();
                        break;
                    }
                    en_expr(expr, pop());
                    en_expr(expr, ' ');
                }
            } else {
                if (c == '-') {       /*  判断是负号还是减号  */
                    if (isdigit(c = getchar())) {
                        en_expr(expr, '-');
                        en_expr(expr, c);
                        continue;
                    } else {
                        ungetc(c, stdin);
                        c = '-';
                    }
                }
                while (!isempty() && top() != '(' && op_prior(top()) >= op_prior(c)) {
                    en_expr(expr, pop());
                    en_expr(expr, ' ');
                }
                push(c);
            }
        }
        lastc = c;
    }
    en_expr(expr, c);
}

int
op_prior(int c)
{
    switch (c) {
    case '+':
    case '-':
        return 1;
        break;
    case '*':
    case '/':
    case '%':
        return 2;
        break;
    case '(':
        return 3;
        break;
    default:
        return 0;
        break;
    }
}

int first;

/*
 * 获取下一个运算符或操作数
 */
int
getop(char *s)
{
    int c;

    if (!first) {
        expr = init_expr();
        first = 1;
    }

    /* 队列为空时，调用exp_trans()读取新的一行 */
    if (is_expr_empty(expr)) {
        clear_expr(&expr);
        expr = init_expr();
        exp_trans();
    }

    while ((s[0] = head_expr(expr)) == ' ' || s[0] == '\t')
        de_expr(expr);
    s[1] = '\0';
    if (!isdigit(c = head_expr(expr)) && !islower(c) && c != '.' && c != '-') {
        c = head_expr(expr);
        de_expr(expr);
        return c;
    }

    if (head_expr(expr) == '-') {      /*  判断‘-’是减号还是负号  */
        de_expr(expr);
        if (isdigit(c = head_expr(expr)) || c == '.')
            *++s = c;
        else
            return '-';
    }     /*  收集数  */
    if (isdigit(head_expr(expr)))
        for (de_expr(expr); isdigit(*++s = head_expr(expr)); de_expr(expr))
            ;
    if (head_expr(expr) == '.')
        for (de_expr(expr); isdigit(*++s = head_expr(expr)); de_expr(expr))
            ;
    *s = '\0';
    return NUMBER;
}
