//#pragma once
#pragma comment(lib "BTICARD.lib")
#include <stdint.h>
#include <string>
#include <cstring>

#include <iostream>
#include <string.h>
#include <strings.h>
#include <sstream>
#include <stdexcept>
#include <bitset>
#include <iomanip>
#include <cstdint>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <unordered_map>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
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

#include "..\..\..\BTICard.h"
#include "..\..\..\BTICARD.lib"

//*	Definitions
//? WebFB Default Values
#define DEFAULT_IP	  "172.31.1.1"
#define DEFAULT_PORT  "10001"
#define MAXPKT		  16384
#define MAX_WPS    	  8192
#define TIMEOUT		  5

// Typedefs
typedef double latitude_t;
typedef unsigned long ulong32_t;
typedef struct manage_t {
	std::uint16_t  buf[MAXPKT];    // Data Buffer
} manage_t;

/*
// Working Testers
int32_t clib_add_internal(int32_t left, int32_t right);
int32_t clib_sub_internal(int32_t left, int32_t right);
char* clib_str_internal();
//////////////////////////////////////////////////////////

class WebFB {
	int num;
	std::string str;
public:
	WebFB();
	WebFB(int n, std::string s);

	int getNum();
	char* getStr();

	void setNum(int n);
	void setStr(std::string s);
};
*/

//////////////////////////////////////////////////////////

// WebFB Class
class WebFB {
public:
    manage_t data;

private:
    int	            sockError;
    sigset_t    	sockSigMask;
    std::size_t		sockFD;
    std::string    	sockIP;
    std::uint16_t  	sockPort;

public:
    WebFB();
    WebFB(std::string IP, std::string Port);
    ~WebFB();

    bool mkSock();
    int sockConnect();
    int initSockPoll();

    int getErr();
};


