#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include "myls.h"

int fname_width;      /*  文件名的输出宽度  */
int fsize_width;      /*  文件大小的输出宽度  */
int flink_width;      /*  文件链接数的输出宽度  */
int funame_width;     /*  文件用户名的输出宽度  */
int fgname_width;     /*  文件所属组名的输出宽度  */
int fuid_width;       /*  文件uid的输出宽度  */
int fgid_width;       /*  文件gid的输出宽度  */
int finode_width;     /*  文件inode的输出宽度  */

/*
 * 获得pathname下的文件总数
 */
int
get_fn(char *pathname)
{
    DIR             *dp;
    struct dirent   *dirp;
    int             fn;

    fn = 0;
    if ((dp = opendir(pathname)) == NULL)     /*  打开目录  */
        err_sys("opendir error");
    if (strcmp(pathname, ".") != 0)
        printf("%-s:\n", pathname);
    while ((dirp = readdir(dp)) != NULL) {    /*  读取目录  */
        if (!(option & FILE_ALL))
            if (dirp->d_name[0] == '.')       /*  跳过隐藏文件  */
                continue;
        fn++;     /*  统计目录下的文件数  */
    }
    closedir(dp);
    return fn;
}

/*
 * 计算出文件名的输出宽度
 */
void
get_fname_width(int ws_col, stat_t *statfp[], int fn)
{
    fname_width = 0;
    for (int i = 0; i < fn; i++)    /*  获取最大文件名的长度  */
        if (strlen(statfp[i]->buf) > fname_width)
            fname_width = strlen(statfp[i]->buf);
}

/*
 * 计算出除文件名以外的其他所需的输出宽度
 */
void
get_other_width(stat_t *statfp[], int fn)
{
    char tmp[100];
    struct passwd   *usr;
    struct group    *grp;

    fsize_width = funame_width = fgname_width = fuid_width = fgid_width = finode_width = 0;
    for (int i = 0; i < fn; i++) {
        sprintf(tmp, "%lld", statfp[i]->statbuf.st_size);
        if (strlen(tmp) > fsize_width)
            fsize_width = strlen(tmp);
        sprintf(tmp, "%d", statfp[i]->statbuf.st_nlink);
        if (strlen(tmp) > flink_width)
            flink_width = strlen(tmp);
        sprintf(tmp, "%u", statfp[i]->statbuf.st_uid);
        if (strlen(tmp) > fuid_width)
            fuid_width = strlen(tmp);
        sprintf(tmp, "%u", statfp[i]->statbuf.st_gid);
        if (strlen(tmp) > fgid_width)
            fgid_width = strlen(tmp);
        sprintf(tmp, "%llu", statfp[i]->statbuf.st_ino);
        if (strlen(tmp) > finode_width)
            finode_width = strlen(tmp);
        if ((usr = getpwuid(statfp[i]->statbuf.st_uid)) == NULL)
            err_sys("getpwuid error");
        if (strlen(usr->pw_name) > funame_width)
            funame_width = strlen(usr->pw_name);
        if ((grp = getgrgid(statfp[i]->statbuf.st_gid)) == NULL)
            err_sys("getgrgid error");
        if (strlen(grp->gr_name) > fgname_width)
            fgname_width = strlen(grp->gr_name);
    }
}
