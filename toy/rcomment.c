#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void in_dcomment(void);
void in_scomment(void);
void echo_quote(int c);

/*
 * 删除c程序中的注释，我们假设输入的都是合法的c程序
 */
int
main(void)
{
    int c, d;

    while ((c = getchar()) != EOF)
        if (c == '/') {
            if ((d = getchar()) == '*')
                in_dcomment();
            else if (d == '/')
                in_scomment();
            else {
                putchar(c);
                putchar(d);
            }
        } else if (c == '\'' || c == '\"')
            echo_quote(c);
        else
            putchar(c);
    return 0;
}

/*
 * 处理//... 注释
 */
void
in_scomment(void)
{
    int c;

    while ((c = getchar()) != '\n')
        if (c == '\\')   /* 处理//注释，或者//用\连接为多行注释的情况  */
            while ((c = getchar()) != '\n')
                if (!isspace(c)) {
	                ungetc(c, stdin);   /*  压回多读入的字符，可能是\  */
                    break;
	             }
}

// 处理/* ...  */注释
void
in_dcomment(void)
{
    int c, d;

    c = getchar();
    d = getchar();
    while (c != '*' || d != '/') {
        c = d;
        d = getchar();
    }
}

/*
 * 原样输出引号内的字符
 */
void
echo_quote(int c)
{
    int d;

    putchar(c);
    while ((d = getchar()) != c) {
        putchar(d);
        if (d == '\\')   /* 正确处理转义字符 */
            putchar(getchar());
    }
    putchar(d);
}
