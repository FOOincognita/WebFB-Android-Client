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

// Functions provided by ballard; used in Parse Packets
ERRVAL BTIUTIL_SeqFindCheckValidType(UINT16 seqtype);
ERRVAL BTIUTIL_SeqFindNext(LPUINT16* pRecord, LPUINT16 seqtype, LPSEQFINDINFO sfinfo);

// std::string to unsigned long; for proper port conversion
#define strtoui16(x)  \
        (std::uint16_t)strtoul(std::string(x).c_str(), NULL, 0) 

#define UINT16_PORT strtoui16(DEFAULT_PORT)

// void on_sigabrt(int signum); USED TO CATCH SIGABRT

enum 
{ 
    POLL_FAIL =    -3,  // WebFB::initSockPoll() failed flag
    CONN_FAIL =    -2,  // WebFB::sockConnect() failed flag
    MKSOCK_FAIL =  -1,  // WebFB::mkSock() failed flag
    ERROR =        -1,  // General Error flag
    SUCCESS =       0,  // General Success flag
    PULSEPKT =      0,  // WebFB::rdSockData() Pulse packet flag
    pTIMEOUT =      0,  // WebFB::sockPoll() Poll timeout flag
    DATA_WAITING =  1,  // WebFB::sockPoll() Data Waiting flag
    NO_DATA =       2   // WebFB::sockPoll() No Data flag
};


// WebFB Class
class WebFB 
{

    error_t	        sockError;      // Socket error value
    sigset_t    	sockSigMask;    // Socket SigMask value
    std::size_t		sockFD;         // Socket file descriptor
    std::string    	sockIP;         // Socket IP
    std::uint16_t  	sockPort;       // Socket Port
    std::uint32_t	sockPKT;        // Socket Packets

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
    ERRVAL SeqFindInit(LPUINT16 seqbuf, UINT32 seqbufsize, LPSEQFINDINFO sfinfo);

    double GetLatData();
};

// SIGABRT Handler
// void try_and_catch_abort(void (*gLat)(WebFB*), WebFB* wfb);

