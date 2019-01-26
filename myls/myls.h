#ifndef _LS_H
#define _LS_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define FILE_ALL    0001     /*  -a  */
#define FILE_LON    0002     /*  -l  */
#define FILE_TIM    0004     /*  -t  */
#define FILE_SIZ    0010     /*  -s  */
#define FILE_UND    0020     /*  -h  */
#define FILE_REV    0040     /*  -r  */
#define FILE_REC    0100     /*  -R  */
#define FILE_NUM    0200     /*  -n  */
#define FILE_INO    0400     /*  -i  */

#define GB      (1024 * 1024 * 1024)
#define MB      (1024 * 1024)
#define KB      (1024)

extern int option;
extern int fname_width;      
extern int fsize_width;      
extern int flink_width;      
extern int funame_width;     
extern int fgname_width; 
extern int fuid_width;       
extern int fgid_width;       
extern int finode_width;     

typedef struct statf {    /*  每个文件的所有信息  */
    struct stat     statbuf;   
    char            buf[FILENAME_MAX + 1];     /*  保存文件名  */
    char            path[PATH_MAX + 1];        /*  保存文件路径  */
} stat_t;

int     (*cmp)(const void *, const void *);
int     filecmp(const void *, const void *);
int     timecmp(const void *, const void *);
int     sizecmp(const void *, const void *);
void    fsize_trans(off_t size);

int     get_fn(char *pathname);
void    get_fname_width(int ws_col, struct statf *statfp[], int fn);
void    get_other_width(struct statf *statfp[], int fn);

void    deal_argfile(int argc, char **argv);
void    show_dir(char *pathname, int fn);
void    show_single_dir(struct statf *statfp[], int fn);
void    show_align(int ws_col, struct statf *staf);
void    show_attribute(struct statf *staf);

void    err_sys(const char *fmt, ...);
void    err_quit(const char *fmt, ...);
void    err_msg(const char *fmt, ...);

#endif    /*  _LS_H  */
