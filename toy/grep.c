/*
 * grep [-n] [pattern] [file ...]
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAXLINE 1000
#define NUMBER  01

static int option;

void printpl(FILE *, char *, char *);

int
main(int argc, char *argv[])
{
    char pattern[MAXLINE];
    FILE *fp;
    int c;

    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch(c) {
            case 'n':
                option |= NUMBER;
                break;
            default:
                goto usage;
            }

    if (argc > 0)
        strncpy(pattern, *argv, sizeof(pattern));
    else
        goto usage;

    if (argc == 1)
        printpl(stdin, "", pattern);
    else
        while (--argc > 0) {
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "can't open %s\n", *argv);
                exit(1);
            }
            printpl(fp, *argv, pattern);
            fclose(fp);
        }
    exit(0);

usage:
    printf("usage: grep [-n] [pattern] [file ...]\n");
    exit(1);
}

void
printpl(FILE *fp, char *fname, char *pattern)
{
    char line[MAXLINE];
    int linenum = 0;

    if (*fname)
        printf("\n - %s:\n", fname);
    while (fgets(line, MAXLINE, fp)) {
        linenum++;
        if (strstr(line, pattern)) {
            if (option & NUMBER)
                printf("%d: ", linenum);
            printf("%s", line);
        }
    }
}
