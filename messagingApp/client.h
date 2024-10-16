#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "user.h"

#pragma comment(lib,"WS2_32")

class Client : public User {
public:
	Client(PCSTR hostAddr, PCSTR hostPort) : User(), _hostAddr(hostAddr), _hostPort(hostPort) {}
	int createSocket(void);

private:
	PCSTR _hostAddr;
	PCSTR _hostPort;
};

#endif