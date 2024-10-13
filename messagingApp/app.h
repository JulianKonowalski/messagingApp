#ifndef APP_H
#define APP_H

#include <iostream>
#include <future>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "client.h"
#include "server.h"

#pragma comment(lib,"WS2_32")

#define SERVER_BUF_LEN 512

class App {
public:
	App(void) {};
	~App(void) { WSACleanup(); }
	void run(void);

private:
	void setupWsa(void);
	void connect(void);
	void awaitConnection(void);
	void connectWithServer(void);
	void chatRoom(void);

	int getUserChoice(void);

	Server _server;
	Client _client;
};

#endif

