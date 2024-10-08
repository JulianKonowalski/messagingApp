#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")


class Server {
public:
	Server(PCSTR port) : _port(port), _socket(INVALID_SOCKET) {}
	~Server(void) { closesocket(_socket); }
	int createSocket(void);
	int startListen(void);
	int acceptConnection(void);

private:
	PCSTR _port;
	SOCKET _socket;
};

#endif
