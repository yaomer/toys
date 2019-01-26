#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <time.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <uuid/uuid.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include "myls.h"

int option = 0;       /*  设置标志位  */

/*
 * ls命令的简要实现，可用于处理[-ahilnRrSt]参数
 */
int
main(int argc, char *argv[])
{
    int c;
    /*  解析输入参数  */
    while (--argc > 0 && (*++argv)[0] == '-')
        while ((c = *++argv[0]))
            switch (c) {
            case 'a':
                option |= FILE_ALL;
                break;
            case 'l':
                option |= FILE_LON;
                break;
            case 't':
                option |= FILE_TIM;
                break;
            case 'S':
                option |= FILE_SIZ;
                break;
            case 'h':
                option |= FILE_UND;
                break;
            case 'r':
                option |= FILE_REV;
                break;
            case 'R':
                option |= FILE_REC;
                break;
            case 'n':
                option |= FILE_NUM;
                break;
            case 'i':
                option |= FILE_INO;
                break;
            default:
                err_quit("usage: ls [-ahilnRrSt] [directories or files...]");
                break;
            }
    if (argc > 0)
        deal_argfile(argc, argv);    /*  处理文件参数表  */
    else
        show_dir(".", get_fn("."));    /*  默认情况下，处理当前目录  */
    exit(0);
}

/*
 * 处理文件列表参数，并判断其是目录还是文件
 */
void
deal_argfile(int argc, char **argv)
{
    DIR             *dp;
    char            *dirbuf[argc];    /* 保存文件参数表中的目录名 */
    stat_t          *statfp[argc];    /* 保存文件参数表中的文件的详细信息 */
    int             dn, fn, i;

    dn = fn = 0;
    while (argc-- > 0)
        if ((dp = opendir(*argv)) == NULL) {   /*  可能是文件  */
            if ((statfp[fn] = malloc(sizeof(stat_t))) == NULL)
                err_sys("malloc error");
            if (lstat(*argv, &statfp[fn]->statbuf) < 0)
                err_sys("lstat error");
            strcpy(statfp[fn++]->buf, *argv++);    /*  这种情况下，statfp中的path[]将会被浪费  */
        } else {
            closedir(dp);   /*  是目录  */
            if ((dirbuf[dn++] = strdup(*argv++)) == NULL)
                err_sys("strdup error");
        }

    if (fn > 0) {     /*  将文件参数表中的所有文件一起打印出来  */
        show_single_dir(statfp, fn);
        if (dn > 0)
            putchar('\n');
        for (i = 0; i < fn; i++)
            free(statfp[i]);
    }

    for (i = 0; i < dn; i++)    /*  处理目录列表  */
        show_dir(dirbuf[i], get_fn(dirbuf[i]));
}

/*
 * 以人类易懂的方式打印文件大小
 */
void
fsize_trans(off_t size)
{
    if (size / GB)
        printf("%4.1lfG ", 1.0 * size / GB);
    else if (size / MB)
        printf("%4.1lfM ", 1.0 * size / MB);
    else if (size / KB)
        printf("%4.1lfK ", 1.0 * size / KB);
    else
        printf("%4lldB ", size);
}
