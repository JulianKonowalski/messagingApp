#ifndef APP_H
#define APP_H

#include <iostream>
#include <future>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "client.h"
#include "server.h"
#include "msgBuffer.h"

#pragma comment(lib,"WS2_32")

#define SERVER_BUF_LEN 512

class App {
public:
	App(void) {};
	~App(void) { WSACleanup(); }
	void run(void);

private:
	void setupWsa(void);
	void welcomeMessage(void);
	void connect(void);
	void connectWithClient(void);
	void connectWithServer(void);
	void chatRoom(void);
	void printMessages(void);
	void receiveMessages(void);
	void sendMessages(void);

	std::string getMessage(void);

	int getUserChoice(void);

	Server _server;
	Client _client;
	MsgBuffer _buffer;
};

#endif

