/*
 * wc [-lwc] [file ...]
 * 忽略了file为目录的情况
 */

#include <stdio.h>
#include <stdlib.h>

#define LINE 01
#define WORD 02
#define CHAR 04

static int option;

void wc(FILE *fp);

int
main(int argc, char *argv[])
{
    int c;
    FILE *fp;

    /* 处理可选参数 */
    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
            case 'l':
                option |= LINE;
                break;
            case 'w':
                option |= WORD;
                break;
            case 'c':
                option |= CHAR;
                break;
            default:
                printf("usage: wc [-lwc] [file ...]\n");
                exit(1);
            }

    /* 处理文件列表 */
    if (argc > 0) {
        while (argc-- > 0) {
            if ((fp = fopen(*argv++, "r")) == NULL) {
                fprintf(stderr, "cat: can't open %s\n", *(argv - 1));
                exit(1);
            }
            wc(fp);
            fclose(fp);
        }
    } else
        wc(stdin);

    exit(0);
}

void
wc(FILE *fp)
{
    enum {OUT, IN};
    int c, nl, nw, nc;
    int state;

    state = OUT;
    nl = nw = nc = 0;
    while ((c = getc(fp)) != EOF) {
        nc++;
        if (c == '\n')
            nl++;
        if (c == ' ' || c == '\t' || c == '\n')
            state = OUT;
        else if (state == OUT) {
            nw++;
            state = IN;
        }
    }

    if (option & LINE)
        printf("%d ", nl);
    if (option & WORD)
        printf("%d ", nw);
    if (option & CHAR)
        printf("%d ", nc);
    if (!option)
        printf("%d %d %d", nl, nw, nc);
    putchar('\n');
}
