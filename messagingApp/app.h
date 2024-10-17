#ifndef APP_H
#define APP_H

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#include "client.h"
#include "msgBuffer.h"
#include "gui.h"

#pragma comment(lib,"WS2_32")

#define SERVER_BUF_LEN 512

class App {
public:
	App(void) {};
	~App(void) { 
		delete _user;
		WSACleanup(); 
	}
	void run(void);

private:
	void welcomeMessage(void);
	void connect(void);
	void hostRoom(void);
	void joinRoom(void);
	void chatRoom(void);
	void printMessages(void);
	void receiveMessages(void);
	void sendMessages(void);

	std::string getUserMessage(void);

	int getUserChoice(void);

	User* _user = nullptr;
	MsgBuffer _buffer;
	GUI gui;
};

#endif

