#ifndef _CODER_H
#define _CODER_H

#include <limits.h>
#include <string>

// [_type] [_path] [_macAddr]\r\n REQ_LINE
// [_fileSize: size]\r\n REQ_HEADER
// \r\n
// [Text]
//
enum STATE { 
    REQ_LINE,   // 请求行
    REQ_HEADER, // 请求头部
    REQ_RECVING,  // 正在接收文件
    REQ_SENDING,  // 正在发送文件
};

struct Request {
    Request() 
        : _state(REQ_LINE), _fd(-1), _filesize(0) {  }
    ~Request() {  }
    int _state;
    int _fd;
    size_t _filesize;
    std::string _type;
    std::string _path;
    std::string _macAddr;
};

#endif // _CODER_H
