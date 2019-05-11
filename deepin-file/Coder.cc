#include <cctype>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include "Channel.h"
#include "Buffer.h"
#include "Coder.h"

void fileMkdir(const char *pathname, const char *mac)
{
    std::string t("./backupfile/");
	t += mac;
	t += "/";
    t += pathname;

    auto t2 = t.begin() + 3;
    while (t2 <= t.end()) {
        for ( ; t2 != t.end(); t2++) {
            if(*t2 == '/')
                break;
        }
        if (t2 >= t.end())
            break;
        char c[100];
        bzero(c, sizeof(c));
        strncpy(c, t.c_str(), t2 - t.begin());
        mkdir(c, 0777);
        t2++;
    }
}

void skipSpace(char *p, char *ep)
{
    while (p < ep && (*p == ' ' || *p == '\t'))
        p++;
}

void parseLine(Request& req, Buffer& buf, int len)
{
    char *p = buf.peek();
    char *ep = p + len;

    skipSpace(p, ep);
    while (p < ep && !isspace(*p))
        req._type.push_back(*p);
    skipSpace(p, ep);
    while (p < ep && !isspace(*p))
        req._path.push_back(*p);
    skipSpace(p, ep);
    while (p < ep && !isspace(*p))
        req._macAddr.push_back(*p);
    req._state = Request::HEADER;
}

void parseHeader(Request& req, Buffer& buf, int len)
{
    char *p = buf.peek();
    char *ep = p + len;

    if (strcmp(p, "\r\n") == 0) {
        if (req._type == "SAVE")
            req._state = Request::OK | Request::RECVING;
        else if (req._type == "GET")
            req._state = Request::OK;
    }
    if (strncasecmp(p, "filesize:", 9) == 0) {
        p += 9;
        skipSpace(p, ep);
        req._filesize = atoi(p);
    } else {
        ; // 目前头部只有一个字段
    }
}

// 解析客户请求
void parseRequest(Buffer& buf, Request& req)
{
    // 可能有一行完整的消息
    while (buf.readable() >= 2) {
        int crlf = buf.findCrlf();
        std::cout << crlf << std::endl;
        // 至少有一行请求
        if (crlf >= 0) {
            if (req._state == Request::LINE) {
                parseLine(req, buf, crlf);
                crlf += 2;
            } else if (req._state == Request::HEADER) {
                parseHeader(req, buf, crlf);
                crlf += 2;
            } else
                break;
            buf.retrieve(crlf);
        } else
            break;
    }
}

std::string getPathname(Request& req)
{
    std::string pathname("./");
    pathname += req._macAddr + req._path;
    return pathname;
}

// SAVE OK\r\n
void replySaveOk(Channel *chl)
{
    std::string s("SAVE OK\r\n\r\n");
    chl->send(s.c_str(), s.size());
}

// GET OK path\r\n
// filesize: size\r\n
// \r\n
// Text
void replyGetOk(Channel *chl, Request& req)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%zu", req._filesize);
    std::string s("GET OK ");
    s += req._path;
    s += "\r\n";
    s += "filesize: ";
    s += buf;
    s += "\r\n";
    chl->send(s.c_str(), s.size());
}

// 接收客户端发来的文件以更新本地文件
void recvFile(Channel *chl, Buffer& buf, Request& req)
{
    std::string pathname = getPathname(req);
    pathname += req._macAddr + req._path;
    if (req._fd < 0) {
        req._fd = open(pathname.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
        req._remainFilesize = req._filesize;
    }
    size_t n = buf.readable();
    if (n > req._remainFilesize) {
        n = req._remainFilesize;
        req._state = Request::LINE;
    } else
        req._remainFilesize -= n;
    write(req._fd, buf.peek(), n);
    buf.retrieve(n);
    if (req._state == Request::LINE) {
        replySaveOk(chl);
        close(req._fd);
        req._fd = -1;
    }
}

// 将备份文件发送给客户
void sendFile(Channel *chl, Request& req)
{
    Buffer buf;
    std::string pathname = getPathname(req);
    int fd = open(pathname.c_str(), O_RDONLY);
    while (buf.readFd(fd) > 0) {
        chl->send(buf.peek(), buf.readable());
        buf.retrieveAll();
    }
    close(fd);
}

// 向客户回复响应信息
void replyResponse(Channel *chl, Buffer& buf, Request& req)
{
    if (req._type == "SAVE") {
        recvFile(chl, buf, req);
    } else if (req._type == "GET") {
        replyGetOk(chl, req);
        sendFile(chl, req);
    }

}

void printInfo(Request& req)
{
    std::cout << "TYPE:     " << req._type
              << "PATH:     " << req._path
              << "MAC:      " << req._macAddr
              << "FILESIZE: " << req._filesize
              << std::endl;
}

void onMessage(std::shared_ptr<Channel> chl, Buffer& buf, Request& req)
{
    std::cout << buf.c_str();
    Channel *chlptr = chl.get();
    if (req._state & Request::RECVING) {
        // 继续接收剩余的文件
        recvFile(chlptr, buf, req);
        return;
    } else {
        // 解析新到来的请求
        parseRequest(buf, req);
    }
    if (req._state & Request::OK) {
        printInfo(req);
        fileMkdir(req._path.c_str(), req._macAddr.c_str());
        replyResponse(chlptr, buf, req);
    }
}
