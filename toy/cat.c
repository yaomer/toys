/*
 * cat [file ...]
 */

#include <stdio.h>
#include <stdlib.h>

void fcopy(FILE *, FILE *);

int main(int argc, char *argv[])
{
    FILE *fp = NULL;

    if (argc == 1)
        fcopy(stdin, stdout);
    else
        while (--argc > 0) {
            if ((fp = fopen(*++argv, "r")) == NULL) {
                fprintf(stderr, "cat: can't open %s\n", *argv);
                exit(1);
            }
            fcopy(fp, stdout);
            fclose(fp);
        }

    if (ferror(stdout)) {
        fprintf(stderr, "wc: error writing stdout\n");
        exit(1);
    }
    exit(0);
}

void
fcopy(FILE *ifp, FILE *ofp)
{
    int c;

    while ((c = getc(ifp)) != EOF)
        putc(c, ofp);
}
