#ifndef SERVER_H
#define SERVER_H

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")

class Server {
public:
	Server(void);
	Server(int bufLen);
	~Server(void) { closesocket(_socket); }
	int createSocket(PCSTR port);
	int startListen(void);
	int connect(void);
	int receiveMsg(void);

private:
	PCSTR _port;
	SOCKET _socket = INVALID_SOCKET;
	SOCKET _clientSocket = INVALID_SOCKET;
	const int _bufLen;
	char* _recvBuf = nullptr;
};

#endif
