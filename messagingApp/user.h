#ifndef USER_H
#define USER_H

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")

class User {

public:
	User(void);
	virtual ~User(void) { closesocket(_socket); }
	virtual int createSocket(void) = 0;

	std::string receiveMsg(void);
	void sendMsg(const char* msg);

protected:
	SOCKET _socket;
	const int _bufLen;
	char* _recvBuf = nullptr;
};

#endif


