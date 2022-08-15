#pragma once

#include <stdint.h>
#include <string>
#include <cstring>
#include <iostream>
#include <string>
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
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <cstring>
#include <vector>

//*	Definitions
//? WebFB Default Values
#define DEFAULT_IP	  "172.31.1.1"
#define DEFAULT_PORT  "10001"
#define MAXPKT		  16384
#define MAX_WPS    	  8192
#define TIMEOUT		  5

//? Console Text Color
#define C           "\033[1;36m"	// Cyan
#define R           "\033[1;31m" 	// Red
#define G           "\033[1;32m" 	// Green
#define M           "\033[1;35m"	// Magenta 
#define RST         "\033[0m" 	    // Reset

// Typedefs
typedef double latitude_t;
typedef unsigned long ulong32_t;
typedef struct manage_t {
	struct termios oldattr;        // Console control (init & restore)
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

// Converts string to uint16_t
std::uint16_t stous(std::string _str) {
	return (std::uint16_t)strtoul(_str.c_str(), NULL, 0);
}

// WebFB Class
class WebFB {
public:
    manage_t data;

private:
    sigset_t    	sockSigMask;
    std::size_t		sockFD;
    std::string    	sockIP;
    std::uint16_t  	sockPort;
    std::uint32_t	sockPKT;
    std::uint32_t	sockError;

public:
    WebFB();
    WebFB(std::string IP, std::string Port);
    ~WebFB();

    int mkSock();
    int sockConnect();
    int initSockPoll();

    int getErr();
};


