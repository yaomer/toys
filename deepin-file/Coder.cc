#include "Channel.h"
#include "Buffer.h"
#include "Coder.h"

void parseLine(Request& req, Buffer& buf, int len)
{

}

void parseHeader(Request& req, Buffer& buf, int len)
{

}

void execRequest(Request& req)
{

}

void replyResponse(Request& req)
{

}

void Channel::handleMessage(Channel& chl)
{
    // 可能有一行完整的消息
    while (chl._input.readable() >= 2) {
        int crlf = chl._input.findCrlf();
        // 至少有一行请求
        if (crlf >= 0) {
            switch (chl._req._state) {
            case REQ_LINE:
                parseLine(chl._req, chl._input, crlf);
                crlf += 2;
                break;
            case REQ_HEADER:
                parseHeader(chl._req, chl._input, crlf);
                crlf += 2;
                break;
            default:
                crlf = chl._input.readable();
                break;
            }
            chl._input.retrieve(crlf);
        }
    }
    replyResponse(chl._req);
}
