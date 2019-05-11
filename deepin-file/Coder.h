#ifndef _CODER_H
#define _CODER_H

#include <string>

// [_type] [_path] [_macAddr]\r\n REQ_LINE
// [_fileSize: size]\r\n REQ_HEADER
// \r\n
// [Text]
//

struct Request {
    enum {
        LINE    = 001,  // 请求行
        HEADER  = 002, // 请求头部
        RECVING = 004, // 正在接收文件
        OK      = 010, // 解析成功
    };
    int _state = LINE;
    int _fd = -1;
    size_t _filesize = 0;
    // SAVE 剩余可读的文件
    size_t _remainFilesize = 0;
    std::string _type;
    std::string _path;
    std::string _macAddr;
};

#endif // _CODER_H
