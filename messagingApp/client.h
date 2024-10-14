#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")

class Client {
public:
	Client(void);
	~Client(void) { closesocket(_socket); }
	int createSocket(PCSTR serverAddr, PCSTR serverPort);
	int sendMsg(const char* msg);

	std::string receiveMsg(void);

	SOCKET getSocket(void) { return _socket; }

private:
	PCSTR _serverAddr;
	PCSTR _serverPort;
	SOCKET _socket;
	const int _bufLen;
	char* _recvBuf = nullptr;
};

#endif