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
#include <sys/types.h>
#include "myls.h"

/*  show_single_dir函数：输出单个目录下的内容  */
void show_single_dir(stat_t *statfp[], int fn)
{
    struct winsize  size;     /*  终端窗口的大小  */
    int             i;

    if (isatty(1)) {     /*  输出至终端  */
        if (ioctl(1, TIOCGWINSZ, &size) < 0)    /*  获取终端窗口的大小  */
            err_sys("ioctl error");
        get_fname_width(size.ws_col, statfp, fn);    /*  获得文件名的输出宽度  */
    } else
        fname_width = 0;    /*  输出重定向到了一个文件  */

    get_other_width(statfp, fn);     /*  获得除文件名以外的其他输出宽度  */

    cmp = ((option & FILE_SIZ) ? sizecmp : ((option & FILE_TIM) ? timecmp : filecmp));
    qsort(statfp, fn, sizeof(statfp[0]), cmp);     /*  根据可选参数对文件进行排序  */

    for (i = 0; i < fn; i++)     /*  打印文件列表  */
        if (option & (FILE_LON | FILE_NUM))
            show_attribute(statfp[i]);
        else
            show_align(size.ws_col, statfp[i]);
    if (!(option & (FILE_LON | FILE_NUM)))
        putchar('\n');
}

/*
 * 如果输出到终端，则对齐输出
 * 如果输出重定向到了一个文件，则一行输出一个
 */

/*  show_align函数：将目录下的所有文件按列对齐输出  */
void show_align(int ws_col, stat_t *staf)
{
    static int row;

    if (fname_width) {
        if (option & FILE_INO) {    /*  文件名前加上inode编号  */
            if ((row += (fname_width + finode_width)) > ws_col) {
                putchar('\n');
                row = fname_width + finode_width + 1;
            } else
                row += 1;
        } else {
            if ((row += fname_width) > ws_col) {
                putchar('\n');
                row = fname_width + 1;    /*  文件间以空格隔开  */
            } else
                row += 1;
        }
    }

    if (option & FILE_INO)
        printf("%*llu ", finode_width, staf->statbuf.st_ino);

    printf("%-*s ", fname_width, staf->buf);
}

/*  show_attribute函数：打印文件的详细属性信息  */
void show_attribute(stat_t *staf)
{
    struct passwd   *usr;
    struct group    *grp;
    char            *timeptr;

    /*  打印文件的inode编号  */
    if (option & FILE_INO)
        printf("%*llu ", finode_width, staf->statbuf.st_ino);

    /*  打印文件的类型信息  */
    if (S_ISREG(staf->statbuf.st_mode))
        printf("-");
    else if (S_ISDIR(staf->statbuf.st_mode))
        printf("d");
    else if (S_ISLNK(staf->statbuf.st_mode))
        printf("l");
    else if (S_ISCHR(staf->statbuf.st_mode))
        printf("c");
    else if (S_ISBLK(staf->statbuf.st_mode))
        printf("b");
    else if (S_ISFIFO(staf->statbuf.st_mode))
        printf("f");
    else if (S_ISSOCK(staf->statbuf.st_mode))
        printf("s");

    /*  打印文件的权限信息  */
    printf("%c", staf->statbuf.st_mode & S_IRUSR ? 'r' : '-');
    printf("%c", staf->statbuf.st_mode & S_IWUSR ? 'w' : '-');
    printf("%c", staf->statbuf.st_mode & S_IXUSR ? 'x' : '-');
    printf("%c", staf->statbuf.st_mode & S_IRGRP ? 'r' : '-');
    printf("%c", staf->statbuf.st_mode & S_IWGRP ? 'w' : '-');
    printf("%c", staf->statbuf.st_mode & S_IXGRP ? 'x' : '-');
    printf("%c", staf->statbuf.st_mode & S_IROTH ? 'r' : '-');
    printf("%c", staf->statbuf.st_mode & S_IWOTH ? 'w' : '-');
    printf("%c", staf->statbuf.st_mode & S_IXOTH ? 'x' : '-');

    /*  打印文件的链接数  */
    printf("  %*d ", flink_width, staf->statbuf.st_nlink);

    /*  打印文件的所有者和用户组  */
    if (option & FILE_NUM) {
        printf("%*d  ", fuid_width, staf->statbuf.st_uid);
        printf("%*d  ", fgid_width, staf->statbuf.st_gid);
    } else {
        if ((usr = getpwuid(staf->statbuf.st_uid)) == NULL)
            err_sys("getpwuid error");
        if ((grp = getgrgid(staf->statbuf.st_gid)) == NULL)
            err_sys("getgrgid error");
        printf("%-*s  ", funame_width, usr->pw_name);
        printf("%-*s  ", fgname_width, grp->gr_name);
    }

    /*  打印文件的大小  */
    if (option & FILE_UND)
        fsize_trans(staf->statbuf.st_size);
    else
        printf("%*lld ", fsize_width, staf->statbuf.st_size);

    /*  打印文件的时间  */
    if ((timeptr = ctime(&staf->statbuf.st_mtime)) == NULL)
        err_sys("ctime error");
    if (timeptr[strlen(timeptr) - 1] == '\n')
        timeptr[strlen(timeptr) - 1] = '\0';
    printf("%s ", timeptr);

    printf("%-s\n", staf->buf);
}

/*  show函数：列出目录下的文件列表  */
void show_dir(char *pathname, int fn)
{
    DIR             *dp;
    struct dirent   *dirp;
    stat_t          *statfp[fn];
    int             i;

    for (i = 0; i < fn; i++)    /*  为数组statfp分配内存  */
        if ((statfp[i] = malloc(sizeof(stat_t))) == NULL)
            err_sys("malloc error");

    if ((dp = opendir(pathname)) == NULL)
        err_sys("opendir error");
    for (i = 0; (dirp = readdir(dp)) != NULL; i++) {
        if (!(option & FILE_ALL))
            if (dirp->d_name[0] == '.') {
                i--;
                continue;
            }      /*  保存目录下每个文件的详细信息  */
        strcpy(statfp[i]->buf, dirp->d_name);
        strcpy(statfp[i]->path, pathname);
        strcat(statfp[i]->path, "/");
        strcat(statfp[i]->path, statfp[i]->buf);
        if (lstat(statfp[i]->path, &statfp[i]->statbuf) < 0)
            err_sys("lstat error");
    }
    closedir(dp);

    show_single_dir(statfp, fn);    /*  打印单个目录下的所有内容  */

    if (option & FILE_REC)    /*  递归遍历目录  */
        for (i = 0; i < fn; i++)
            if (strcmp(statfp[i]->buf, ".") != 0
            && strcmp(statfp[i]->buf, "..") != 0
            && S_ISDIR(statfp[i]->statbuf.st_mode))
                show_dir(statfp[i]->path, get_fn(statfp[i]->path));

    for (i = 0; i < fn; i++)    /*  释放数组statfp的内存  */
        free(statfp[i]);
}
