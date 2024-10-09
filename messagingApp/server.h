#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")


class Server {
public:
	Server(PCSTR port, int bufLen);
	~Server(void) { closesocket(_socket); }
	int createSocket(void);
	int startListen(void);
	int acceptConnection(void);
	int reveiveMsg(void);

private:
	PCSTR _port;
	SOCKET _socket = INVALID_SOCKET;
	SOCKET _clientSocket = INVALID_SOCKET;
	const int _bufLen;
	char* _recvBuf = nullptr;
};

#endif
