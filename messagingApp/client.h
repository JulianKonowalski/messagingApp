#ifndef CLIENT_H
#define CLIENT_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")

class Client {
public:
	Client(PCSTR serverAddr, PCSTR serverPort) : _serverAddr(serverAddr), _serverPort(serverPort), _socket(INVALID_SOCKET) {}
	~Client(void) { closesocket(_socket); }
	int createSocket(void);

private:
	PCSTR _serverAddr;
	PCSTR _serverPort;
	SOCKET _socket;
};

#endif