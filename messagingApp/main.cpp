#include <iostream>

#include "wsaInit.h"
#include "client.h"
#include "server.h"

int main(void) {

	if (initialiseWSA(MAKEWORD(2, 2)) != 0) {
		WSACleanup();
		return 0;
	}

	int choice;
	std::cin >> choice;

	Server server("8080");
	Client client("192.168.1.246", "8080");

	switch (choice) {
	case 1:
		server.createSocket();
		server.startListen();
		server.acceptConnection();
		break;
	case 2:
		client.createSocket();
		break;
	}

	system("pause");
	return 0;
}