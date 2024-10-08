#include "wsaInit.h"

#include <iostream>

int initialiseWSA(WORD winSockVersion) {
	std::cout << "Initialising WinSock2" << std::endl;

	WSADATA wsaData;

	int wsaResult = WSAStartup(winSockVersion, &wsaData);
	if (wsaResult != 0) {
		std::cout << "WinSock2 failed to initialise" << std::endl;
		std::cout << "Error code: " << wsaResult << std::endl;
		return 1;
	}

	std::cout << "WinSock2 initialised successfully" << std::endl;
	std::cout << "Winsock2 Status: " << wsaData.szSystemStatus << "\n" << std::endl;
	return 0;
}