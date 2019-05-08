#ifndef _CODER_H
#define _CODER_H

#include <limits.h>

// [_type] [_path] [_macAddr]\r\n REQ_LINE
// [_fileSize: size]\r\n REQ_HEADER
// \r\n
// Text

struct Request {
    enum STATE { REQ_LINE, REQ_HEADER, REQ_OK, REQ_ERROR };
    int _state = REQ_LINE;
    // 请求类型
    char _type[8];
    // 请求的文件路径
    char _path[PATH_MAX + 1];
    // 请求客户机的mac地址
    char _macAddr[32];
    // 文件大小
    size_t _fileSize;
};

#endif // _CODER_H
