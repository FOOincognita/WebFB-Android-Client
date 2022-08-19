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

#include <csetjmp>
#include <csignal>
#include <cstdlib>
#include <iostream>

#include <BTICard.h>
#include "global.h"

ERRVAL BTIUTIL_SeqFindCheckValidType(UINT16 seqtype);
ERRVAL BTIUTIL_SeqFindNext(LPUINT16* pRecord, LPUINT16 seqtype, LPSEQFINDINFO sfinfo);

#define strtoui16(x) (std::uint16_t)strtoul(std::string(x).c_str(), NULL, 0)
#define UINT16_PORT strtoui16(DEFAULT_PORT)

// void on_sigabrt(int signum);

enum { 
    POLL_FAIL =    -3,
    CONN_FAIL =    -2,
    MKSOCK_FAIL =  -1,
    ERROR =        -1,  
    SUCCESS =       0,
    PULSEPKT =      0, 
    pTIMEOUT =      0, 
    DATA_WAITING =  1, 
    NO_DATA =       2 
};


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

    std::int32_t SeqFindInit(LPUINT16 seqbuf, UINT32 seqbufsize, LPSEQFINDINFO sfinfo);

    double GetLatData();
};

// SIGABRT Handler
// void try_and_catch_abort(void (*gLat)(WebFB*), WebFB* wfb);

