#pragma once

#include <stdint.h>
#include <string>
#include <cstring>
#include <poll.h>
#include <math.h>
#include <iostream>
#include <string.h>
#include <strings.h>
#include <sstream>
#include <stdexcept>
#include <bitset>
#include <iomanip>
#include <time.h>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unordered_map>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/poll.h>
#include <sys/eventfd.h>
#include <sys/socket.h>
#include <syslog.h>
#include <arpa/inet.h>
#include <stddef.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/cdefs.h>
#include <linux/poll.h>

#include <BTICard.h>
#include "global.h"

ERRVAL BTIUTIL_SeqFindCheckValidType(UINT16 seqtype);
ERRVAL BTIUTIL_SeqFindNext(LPUINT16* pRecord, LPUINT16 seqtype, LPSEQFINDINFO sfinfo);

#define strtoui16(x) (std::uint16_t)strtoul(std::string(x).c_str(), NULL, 0)
#define UINT16_PORT strtoui16(DEFAULT_PORT)

// WebFB Class
class WebFB {
public:
    manage_t data;

private:
    error_t	        sockError;
    sigset_t    	sockSigMask;
    std::size_t		sockFD;
    std::string    	sockIP;
    std::uint16_t  	sockPort;
    std::uint32_t	sockPKT;

public:
    WebFB();
    WebFB(std::string IP, std::string Port);
    ~WebFB();

    bool mkSock();
    int sockConnect();
    int initSockPoll();

    int getErr();

    int rdSockData(std::uint16_t* pbuf, std::uint32_t bufsize);
    int sockPoll();
    std::string ParsePKTS(LPUINT16 buf, std::uint32_t wordcount, std::string lbl);
    // std::string GetArincData(std::string lbl);
    latitude_t GetLatData();

    std::int32_t SeqFindInit(std::uint16_t* seqbuf, std::uint32_t seqbufsize, LPSEQFINDINFO sfinfo);
};


