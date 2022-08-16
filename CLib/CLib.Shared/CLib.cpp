#include "CLib.h"

//* Default Constructor
WebFB::WebFB()
	: sockIP(DEFAULT_IP), sockPort((std::uint16_t)strtoul(std::string(DEFAULT_PORT).c_str(), NULL, 0)), sockFD(-1), sockError(0), sockPKT(0) {

	if (!this->mkSock()) {
		sockError = -1;
		return;
	}
	else {
		sockError = 0;
	}

	if (!this->sockConnect()) {
		sockError = -2;
		return;
	}
	else {
		sockError = 0;
	}

	if (!this->initSockPoll()) {
		sockError = -3;
		return;
	}
	else {
		sockError = 0;
	}
}

//* Paramaterized Constructor
// @param IP: IP Address
// @param Port: IP Address Port
WebFB::WebFB(std::string IP, std::string Port)
	: sockIP(IP), sockPort((std::uint16_t)strtoul(std::string(DEFAULT_PORT).c_str(), NULL, 0)), sockFD(-1), sockError(0), sockPKT(0) {

	if (!this->mkSock()) {
		sockError = -1;
		return;
	}
	else {
		sockError = 0;
	}

	if (!this->sockConnect()) {
		sockError = -2;
		return;
	}
	else {
		sockError = 0;
	}

	if (!this->initSockPoll()) {
		sockError = -3;
		return;
	}
	else {
		sockError = 0;
	}
}

//* Destructor
//? Closes Socket
WebFB::~WebFB() { close(this->sockFD); }

//* Creates Socket
//? Returns 0 when failure
bool WebFB::mkSock() {
	this->sockFD = socket(AF_INET, SOCK_STREAM, 0);
	return !(this->sockFD == -1);
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

int WebFB::getErr() { return this->sockError; }

//* Reads Data From Socket
//?  -1 = Error
//?  0 = Pulse Packet
//?  >0 = Data Packet
//   @param pbuf: Data Buffer
//   @param bufsize: Size of Buffer in Bytes
int WebFB::rdSockData(std::uint16_t* pbuf, std::uint32_t bufsize) {
	int result(0), j(0);
	std::uint32_t wordcount(0);

	if (!pbuf) { return -1; } //? Error

	result = recv(this->sockFD, (char*)&wordcount, sizeof(wordcount), MSG_WAITALL);

	//Socket error - disconnected?
	if (result <= 0) { return -1; }

	//Unexpected packet
	if (result != sizeof(wordcount)) { return -1; }

	//Switch count from network to host order
	wordcount = ntohl(wordcount);

	//? Pulse packet
	if (!wordcount) { return 0; }

	//? Error: Unexpected packet
	if (wordcount > bufsize) { return -1; }

	result = recv(this->sockFD, (char*)pbuf, wordcount * 2, MSG_WAITALL);

	//? Error: Socket disconnected?
	if (result <= 0) { return -1; }

	//? Error: Unexpected packet
	if (result != (wordcount * 2)) { return -1; }

	this->sockPKT++;

	return wordcount;
}

//* Poll for events (data ready on socket)
//?	-1 = Error
//?   0 = Poll Timeout
//?   1 = Data Waiting
//?   2 = No Data
int WebFB::sockPoll() {
	nfds_t nfds(0);
	int result(0), j(0);
	struct pollfd fds[2];
	struct timespec timeout;

	timeout.tv_sec = TIMEOUT;
	timeout.tv_nsec = 0;

	std::memset(&fds, 0, sizeof(fds));

	fds[0].fd = this->sockFD;
	fds[0].events = POLLIN;
	nfds++;

	//	Poll for signal
	result = ppoll(fds, nfds, &timeout, &(this->sockSigMask));
	if (result > 0) {
		for (j = 0; j < nfds; j++) {
			if (fds[j].revents & POLLIN) {
				if (fds[j].fd == this->sockFD) { //? Data was sent from server
					return 1; //? Data Waiting
				}
			}
		}
		return 2;	//? No Data
	}
	else if (!result) {
		return 0; //? Poll Timeout
	}
	else if (errno == EINTR) {
		return 2; //? No Data
	}
	return -1; //? Error
}

//	Parse data packets read from the socket
//? Returns empty string on failure
std::string WebFB::ParsePKTS(LPUINT16 buf, uint32_t wordcount, std::string lbl) {
	UINT16            seqtype;
	LPUINT16          pRec;
	std::string       hexStr;
	SEQFINDINFO       sfinfo;
	LPSEQRECORD429    pRec429;
	std::stringstream ss;

	if (BTICard_SeqFindInit(buf, wordcount, &sfinfo)) {
		//? syslog(LOG_ERR,"ParsePackets - SeqFindInit Failed (%i)", errval);
		return "";
	}

	//	Walk the record stream using our modified find-next method
	//	printf("%sHEX: 0x%s %s\n", C, hexStr.c_str(), RST);
	while (!BTIUTIL_SeqFindNext(&pRec, &seqtype, &sfinfo)) {
		if (seqtype == SEQTYPE_429) {
			pRec429 = (LPSEQRECORD429)pRec;
			hexStr.clear();
			ss.clear();

			ss << std::hex << ntohl(pRec429->data);
			hexStr = ss.str();

			if (hexStr.size() == 8 && hexStr.substr(hexStr.length() - 2) == lbl) {
				return hexStr;
			}
		}
	}
	return "";
}

// Returns double corresponding to c8, latitude values translated
latitude_t WebFB::GetLatData() {
	int 		result(0);
	std::string rawHex, w32, bit1428Str;

	for (;;) {
		if (this->sockPoll() == 1) {
			result = this->rdSockData(this->data.buf, MAXPKT);
			if (result > 0) {
				rawHex = this->ParsePKTS(this->data.buf, result, std::string("c8"));
				for (auto& i : rawHex) { w32 += hexMap.at(std::to_string(i)); }
				bit1428Str = w32.substr(4, 20);
				return std::stol(bit1428Str.c_str(), nullptr, 2) * 0.00017166154;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////
ERRVAL BTIUTIL_SeqFindCheckValidType(UINT16 seqtype) {
	seqtype &= SEQTYPE_MASK;
	if (seqtype == SEQTYPE_429)  return(ERR_NONE);
	return(ERR_SEQTYPE);
}

ERRVAL BTIUTIL_SeqFindNext(LPUINT16* pRecord, LPUINT16 seqtype, LPSEQFINDINFO sfinfo) {
	ERRVAL errval;
	LPUINT16 pSeqBuf;

	if (!sfinfo) return(ERR_SEQFINDINFO);

	for (pSeqBuf = sfinfo->pRecNext; pSeqBuf < sfinfo->pRecLast;) {
		// Check for a known record type
		errval = BTIUTIL_SeqFindCheckValidType(pSeqBuf[0]);
		if (errval) {
			return(errval);
		}

		//Advance the pointer in the sfinfo struct
		sfinfo->pRecNext += pSeqBuf[1];

		if (pRecord) *pRecord = pSeqBuf;
		if (seqtype) *seqtype = pSeqBuf[0] & SEQTYPE_MASK;

		return(ERR_NONE);
	}
	return(ERR_SEQNEXT);
}