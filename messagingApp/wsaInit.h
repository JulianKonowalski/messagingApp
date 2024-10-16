#ifndef WSAINIT_H
#define WSAINIT_H

#include <iostream>
#include <WinSock2.h>

void initialiseWSA(WORD winSockVersion) {
	WSADATA wsaData;
	int wsaResult = WSAStartup(winSockVersion, &wsaData);
	if (wsaResult != 0) {
		std::cout << "WinSock2 failed to initialise" << std::endl;
		std::cout << "Error code: " << wsaResult << std::endl;
		throw std::runtime_error("Failed to initialise WSA");
	}
}

#endif