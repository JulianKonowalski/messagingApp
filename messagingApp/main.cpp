#include <iostream>

#include "wsaInit.h"
#include "client.h"
#include "server.h"

#define SERVER_BUF_LEN 8

int main(void) {

	if (initialiseWSA(MAKEWORD(2, 2)) != 0) {
		WSACleanup();
		return 0;
	}

	int choice;
	std::cin >> choice;

	Server server("8080", SERVER_BUF_LEN);
	Client client("192.168.1.246", "8080");

	switch (choice) {
	case 1:
		server.createSocket();
		server.startListen();
		server.acceptConnection();
		server.reveiveMsg();
		break;
	case 2:
		std::string msg = "Hello server!";
		client.createSocket();
		client.sendMsg(msg.c_str());
		break;
	}

	system("pause");
	return 0;
}