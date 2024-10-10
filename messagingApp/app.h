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
	int setupWsa(void);
	void connect(void);
	void awaitConnection(bool* connected, std::mutex* connectedMutex);
	void connectWithServer(bool* connected, std::mutex* connectedMutex);
	std::string getServerAddr(void);

	Server _server;
	Client _client;
};

#endif

