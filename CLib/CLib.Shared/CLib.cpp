#include "CLib.h"

/* SIGABRT HANDLER
jmp_buf env;
int val(0);

void on_sigabrt(int signum)
{
	signal(signum, SIG_DFL);
	longjmp(env, 1);
}

void try_and_catch_abort(void (*gLat)(WebFB*), WebFB* wfb)
{
	if (setjmp(env) == 0) {
		signal(SIGABRT, &on_sigabrt);
		gLat(wfb);
		signal(SIGABRT, SIG_DFL);
	}
}
*/

//* Default Constructor
WebFB::WebFB()
	: sockIP(DEFAULT_IP), sockPort(UINT16_PORT), sockFD(-1), sockError(0), sockPKT(0)
{

	if (!this->mkSock()) 
	{
		sockError = -1;
		return;
	}
	if (!this->sockConnect()) 
	{
		sockError = -2;
		return;
	}
	if (!this->initSockPoll()) 
	{
		sockError = -3;
		return;
	}
}

//* Paramaterized Constructor
// @param IP: IP Address
// @param Port: IP Address Port
WebFB::WebFB(std::string IP, std::string Port)
	: sockIP(IP), sockPort(strtoui16(Port)), sockFD(-1), sockError(0), sockPKT(0) 
{

	if (!this->mkSock()) 
	{
		sockError = -1;
		return;
	}
	if (!this->sockConnect()) 
	{
		sockError = -2;
		return;
	}
	if (!this->initSockPoll()) 
	{
		sockError = -3;
		return;
	}
}

//* Destructor
//? Closes Socket
WebFB::~WebFB() { close(this->sockFD); }

//* Creates Socket
//? Returns 0 when failure
bool WebFB::mkSock() 
{
	this->sockFD = socket(AF_INET, SOCK_STREAM, 0);
	return !(this->sockFD == -1);
}

//* Connects to WebFB
//Returns 0 when failure
int WebFB::sockConnect() 
{
	struct sockaddr_in addr;
	int result(0);

	if (this->sockFD == -1) { return 0; }

	std::memset(&addr, 0, sizeof(addr));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(this->sockPort);
	addr.sin_addr.s_addr = inet_addr((this->sockIP.c_str()));

	result = connect(this->sockFD, (struct sockaddr*)&addr, sizeof(addr));
	if (result < 0) { return 0; }

	return 1;
}

//* Setup sig mask for ppoll
//? Returns 0 when failure
int WebFB::initSockPoll() 
{
	return (int)!(sigprocmask(SIG_BLOCK, NULL, &(this->sockSigMask)) < 0);
}

int WebFB::getErr() { return this->sockError; }

//* Reads Data From Socket
//?  -1 = Error
//?  0 = Pulse Packet
//?  >0 = Data Packet
//   @param pbuf: Data Buffer
//   @param bufsize: Size of Buffer in Bytes
int WebFB::rdSockData(std::uint16_t* pbuf, std::uint32_t bufsize) 
{
	int			  result(0);
	std::uint32_t wordcount(0);

	if (!pbuf) { return ERROR; } //? Error

	result = recv(this->sockFD, (char*)&wordcount, sizeof(wordcount), MSG_WAITALL);

	if (result <= 0) { return ERROR; } //? Disconnected?
	if (result != sizeof(wordcount)) { return ERROR; } //? Unexpected packet

	wordcount = ntohl(wordcount); //? Switch count from network to host order

	if (!wordcount) { return PULSEPKT; } //? Pulse packet
	if (wordcount > bufsize) { return ERROR; } //? Error: Unexpected packet

	result = recv(this->sockFD, (char*)pbuf, wordcount * 2, MSG_WAITALL);

	if (result <= 0) { return ERROR; } //? Error: Socket disconnected?
	if (result != (wordcount * 2)) { return ERROR; } //? Error: Unexpected packet

	this->sockPKT++;

	return wordcount;
}

//* Poll for events (data ready on socket)
//?	  -1 = Error
//?   0 = Poll Timeout
//?   1 = Data Waiting
//?   2 = No Data
int WebFB::sockPoll() 
{
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

	result = ppoll(fds, nfds, &timeout, &(this->sockSigMask)); //!!! POLL.H IS A POSSIBLE ERROR SOURCE,
	if (result > 0)										// BUT MSVS PROVIDED THIS ANDROID COMPATIBLE VERSION OF POLL.h
	{													// ERR SOURCE: "_Nullable"
		for (j = 0; j < nfds; j++)						// HOWEVER, THE COMPILER DOESN'T HAVE ANYTHING TO SAY ABOUT THIS
		{									
			if (fds[j].revents & POLLIN) 
			{
				if (fds[j].fd == this->sockFD) { return DATA_WAITING; }
			}
		}
		return NO_DATA;
	} 
	else if (!result) 
	{
		return pTIMEOUT; 
	} 
	else if (errno == EINTR) 
	{
		return NO_DATA; 
	}
	return ERROR; 
}

//	Parse data packets read from the socket
//? Returns empty string on failure
std::string WebFB::ParsePKTS(LPUINT16 buf, uint32_t wordcount, std::string lbl) 
{
	ERRVAL			  errval;
	UINT16            seqtype;
	LPUINT16          pRec;
	std::string       hexStr;
	SEQFINDINFO       sfinfo;
	LPSEQRECORD429    pRec429;
	std::stringstream ss;

	errval = this->SeqFindInit(buf, wordcount, &sfinfo);

	if (errval) { return ""; }

	while (!BTIUTIL_SeqFindNext(&pRec, &seqtype, &sfinfo)) 
	{
		if (seqtype == SEQTYPE_429) 
		{
			pRec429 = (LPSEQRECORD429)pRec;
			hexStr.clear(); ss.clear();

			ss << std::hex << ntohl(pRec429->data);
			hexStr = ss.str();

			if (hexStr.size() == 8 && (hexStr.substr(hexStr.length() - 2) == lbl)) { return hexStr; }
		}
	}
	return "";
}

// Returns double corresponding to c8, latitude values translated
//! ISSUE: THIS FUNCTION THROWS A SIGABRT SIGNAL, CAUSING APP TO CRASH
double WebFB::GetLatData() 
{
	int 		result(0);
	std::string rawHex, w32, bit1428Str, lbl("c8");

	for (;;) 
	{
		if (this->sockPoll() == 1) 
		{
			result = this->rdSockData(buf, MAXPKT);
			if (result > 0) 
			{
				rawHex = this->ParsePKTS(buf, result, lbl);
				for (auto& i : rawHex) { w32 += hexMap.at(i); }
				bit1428Str = w32.substr(4, 20);
				return std::stol(bit1428Str.c_str(), nullptr, 2) * 0.00017166154;
			}
		}
	}
}


/*******************************************************************************************
 * THIS IS BTICard_SeqFindInit() FROM libbticard.so (x86) 
 * DISASSEMBLED INTO INTEL x86 ASM,THEN REASSEMBLED TO C LINE-BY-LINE
 * [WARNING]: AVOID REVERSE ENGINEERING ANY OTHER FUNCTIONS FROM THEIR LIBRARY
 * AS IT IS A VIOLATION OF THE LICENSING AGREEMENT
 *
 *
 *
 *								INTEL x86 (Little Endian)
 *					SYNTAX:
 *						+ var = <size> ptr <stack base offset>
 *						+ mov	dst, src	| Move src into dst
 *						+ mov	[eax], edx	| Addr pointed to by eax <-- Contents of edx
 *						+ mov	ebx, [edx]	| ebx <-- Data @ address pointed to by edx
*/                                                                 
// int                    unsigned short*    unsigned short	  struct SEQFINDINFO*
ERRVAL WebFB::SeqFindInit(LPUINT16 seqbuf, UINT32 seqbufsize, LPSEQFINDINFO sfinfo) {
	/*	
	| 
	*	arg_1 = dword ptr 0x4  |  seqbuf		
	*	arg_2 = dword ptr 0x8  |  seqbufsize	
	*	arg_3 = dword ptr 0xC  |  sfinfo		
	| 
	*/

	if (!sfinfo) { return(ERR_SEQFINDINFO); } 
		/*	
		| 
		*	mov    eax,	[esp+arg_3]  |	eax = sfinfo
		*	mov	   edx, [esp+arg_1]	 |	edx = seqbuf
		*	test   eax, eax			 |	if (sfinfo == 0)
		*	jz	   short loc_6910E	 |  JUMP IFF ZERO-FLAG
		    |
			* loc_6910E:
			    |
				* mov    eax, 0xFFFFFFAE  |  eax = -82
				* retn					  |  return eax
		|
		*/

	sfinfo->pRecFirst = seqbuf;
		/*
		!    PREV:  edx = seqbuf
		|
		*	 mov    ecx, [esp+arg_2]  |  ecx = seqbufsize
		*	 mov    [eax], edx		  |  sfinfo[0] = seqbuf
		|
		*/
	sfinfo->pRecNext = seqbuf;
		/*
		|
		*    mov	[eax+4], edx  |  sfinfo[1] = seqbuf
		|
		*/
	sfinfo->pRecLast = seqbuf + seqbufsize*2;
		/*
		|
		*    lea	edx, [edx+ecx*2]  |  edx = seqbuf + seqbufsize*2
		*    mov    [eax+8], edx      |  sfinfo[2] = seqbuf + seqbufsize*2
		|
		*/
	return (ERR_NONE);
		/*
		|
		*    xor    eax, eax  |  eax = 0 <=> ERR_NONE
		|
		*/
}
//*******************************************************************************************

ERRVAL BTIUTIL_SeqFindCheckValidType(UINT16 seqtype) 
{
	seqtype &= SEQTYPE_MASK;
	if (seqtype == SEQTYPE_429)  return(ERR_NONE);
	return(ERR_SEQTYPE);
}

ERRVAL BTIUTIL_SeqFindNext(LPUINT16* pRecord, LPUINT16 seqtype, LPSEQFINDINFO sfinfo) 
{
	ERRVAL		errval;
	LPUINT16	pSeqBuf;

	if (!sfinfo) return(ERR_SEQFINDINFO);

	for (pSeqBuf = sfinfo->pRecNext; pSeqBuf < sfinfo->pRecLast;) 
	{
		errval = BTIUTIL_SeqFindCheckValidType(pSeqBuf[0]);
		if (errval) return(errval); 

		sfinfo->pRecNext += pSeqBuf[1];

		if (pRecord) *pRecord = pSeqBuf; 
		if (seqtype) *seqtype = pSeqBuf[0] & SEQTYPE_MASK;

		return(ERR_NONE);
	}
	return(ERR_SEQNEXT);
}
