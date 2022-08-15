#include "CLib.h"


/*
int32_t clib_add_internal(int32_t left, int32_t right)
{
    return left + right;
}

int32_t clib_sub_internal(int32_t left, int32_t right)
{
    return left - right;
}

char* clib_str_internal()
{
    return const_cast<char*>("Howdy");
}
//////////////////////////////////////////////////////////

// Internal class funcs
WebFB::WebFB() : num(69), str("Howdy") {}
WebFB::WebFB(int n, std::string s) : num(n), str(s) {}

int WebFB::getNum() { return this->num;  }
char* WebFB::getStr() { return const_cast<char*>((this->str).c_str()); }

void WebFB::setNum(int n) { this->num = n; }
void WebFB::setStr(std::string s) { this->str = s;  }
*/
//////////////////////////////////////////////////////////

//* Default Constructor
WebFB::WebFB()
	: sockIP(DEFAULT_IP), sockPort((std::uint16_t)strtoul(std::string(DEFAULT_PORT).c_str(), NULL, 0)), sockFD(-1), sockError(0) {

	sockError = (!this->mkSock()) ? -1 : 0;
	sockError = (!this->sockConnect()) ? -2 : 0;
	sockError = (!this->initSockPoll()) ? -3 : 0;
}

//* Paramaterized Constructor
// @param IP: IP Address
// @param Port: IP Address Port
WebFB::WebFB(std::string IP, std::string Port)
	: sockIP(IP), sockPort((std::uint16_t)strtoul(std::string(DEFAULT_PORT).c_str(), NULL, 0)), sockFD(-1), sockError(0) {

	sockError = (!this->mkSock()) ? -1 : 0;
	sockError = (!this->sockConnect()) ? -2 : 0;
	sockError = (!this->initSockPoll()) ? -3 : 0;
}

//* Destructor
//? Closes Socket
WebFB::~WebFB() { close(this->sockFD); }

//* Creates Socket
//? Returns 0 when failure
int WebFB::mkSock() {
	this->sockFD = socket(AF_INET, SOCK_STREAM, 0);
	return (int)!(this->sockFD == -1);
}

//* Connects to WebFB
//Returns 0 when failure
int WebFB::sockConnect() {
	struct sockaddr_in addr;
	int result(0);

	if (this->sockFD == -1) { return 0; }

	std::memset(&addr, 0, sizeof(addr));
	//bzero(&addr, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->sockPort);
	addr.sin_addr.s_addr = inet_addr((this->sockIP.c_str()));

	result = connect(this->sockFD, (struct sockaddr*)&addr, sizeof(addr));
	if (result < 0) { return 0; }

	return 1;
}

//* Setup sig mask for ppoll
//? Returns 0 when failure
int WebFB::initSockPoll() {
	return !(sigprocmask(SIG_BLOCK, NULL, &(this->sockSigMask)) < 0);
}

std::uint32_t WebFB::getErr() { return this->sockError; }