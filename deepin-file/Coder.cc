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
        req.type().push_back(*p);
    skipSpace(p, ep);
    while (p < ep && !isspace(*p))
        req.path().push_back(*p);
    skipSpace(p, ep);
    while (p < ep && !isspace(*p))
        req.mac().push_back(*p);
    req.setState(Request::HEADER);
}

void parseHeader(Request& req, Buffer& buf, int len)
{
    char *p = buf.peek();
    char *ep = p + len;

    if (strcmp(p, "\r\n") == 0) {
        if (req.type() == "SAVE")
            req.setState(Request::OK | Request::RECVING);
        else if (req.type() == "GET")
            req.setState(Request::OK);
    }
    if (strncasecmp(p, "filesize:", 9) == 0) {
        p += 9;
        skipSpace(p, ep);
        req.setFilesize(atoi(p));
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
            if (req.state() == Request::LINE) {
                parseLine(req, buf, crlf);
                crlf += 2;
            } else if (req.state() == Request::HEADER) {
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
    pathname += req.mac() + req.path();
    return pathname;
}

// SAVE-OK\r\n
// filesize: 0\r\n
// \r\n
void replySaveOk(Channel *chl)
{
    std::string s("SAVE-OK\r\n");
    s += "filesize: 0\r\n";
    s += "\r\n";
    chl->send(s.c_str(), s.size());
}

// GET-OK path\r\n
// filesize: size\r\n
// \r\n
// Text
void replyGetOk(Channel *chl, Request& req)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%zu\r\n", req.filesize());
    std::string s("GET-OK ");
    s += req.path();
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
    pathname += req.mac() + req.path();
    if (req.fd() < 0) {
        int fd = open(pathname.c_str(), O_WRONLY | O_APPEND | O_CREAT, 0666);
        req.setFd(fd);
    }
    size_t n = buf.readable();
    if (n > req.filesize()) {
        n = req.filesize();
        req.setState(Request::LINE);
    } else
        req.setFilesize(req.filesize() - n);
    write(req.fd(), buf.peek(), n);
    buf.retrieve(n);
    if (req.state() == Request::LINE) {
        replySaveOk(chl);
        close(req.fd());
        req.setFd(-1);
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
    if (req.type() == "SAVE") {
        recvFile(chl, buf, req);
    } else if (req.type() == "GET") {
        replyGetOk(chl, req);
        sendFile(chl, req);
    }

}

void printInfo(Request& req)
{
    std::cout << "TYPE:     " << req.type()
              << "PATH:     " << req.path()
              << "MAC:      " << req.mac()
              << "FILESIZE: " << req.filesize()
              << std::endl;
}

void onMessage(std::shared_ptr<Channel> chl, Buffer& buf)
{
    std::cout << buf.c_str();
    Channel *chlptr = chl.get();
    Request& req = chl->req();
    if (req.state() & Request::RECVING) {
        // 继续接收剩余的文件
        recvFile(chlptr, buf, req);
        return;
    } else {
        // 解析新到来的请求
        parseRequest(buf, req);
    }
    if (req.state() & Request::OK) {
        printInfo(req);
        fileMkdir(req.path().c_str(), req.mac().c_str());
        replyResponse(chlptr, buf, req);
    }
}
