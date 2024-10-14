#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")

class Client {
public:
	Client(void) : _serverAddr(NULL), _serverPort(NULL), _socket(INVALID_SOCKET) {}
	~Client(void) { closesocket(_socket); }
	int createSocket(PCSTR serverAddr, PCSTR serverPort);
	int sendMsg(const char* msg);

	SOCKET getSocket(void) { return _socket; }

private:
	PCSTR _serverAddr;
	PCSTR _serverPort;
	SOCKET _socket;
};

#endif