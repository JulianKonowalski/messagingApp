#ifndef USER_H
#define USER_H

#define DEFAULT_BUF_LEN 128
#define SHUTDOWN_REQUEST_FLAG "\0"
#define SHUTDOWN_CONFIRMATION_FLAG "\1"

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
	void shutdown(void);

	bool isActive(void) { return _active; }

protected:
	SOCKET _socket;
	const int _bufLen;
	char* _recvBuf = nullptr;
	bool _active;
};

#endif


