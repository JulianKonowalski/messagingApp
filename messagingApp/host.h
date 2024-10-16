#ifndef HOST_H
#define HOST_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>

#include "user.h"

#pragma comment(lib,"WS2_32")

class Host : public User {
public:
	Host(PCSTR port) : User(), _port(port) {}
	~Host(void) override { closesocket(_hostSocket); }
	int createSocket(void);
	int startListen(void);
	int connect(void);

	SOCKET getSocket(void) { return _socket; }

private:
	PCSTR _port;
	SOCKET _hostSocket = INVALID_SOCKET;
};

#endif
