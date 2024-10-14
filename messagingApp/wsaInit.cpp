#include "wsaInit.h"

#include <iostream>

int initialiseWSA(WORD winSockVersion) {
	WSADATA wsaData;
	int wsaResult = WSAStartup(winSockVersion, &wsaData);
	if (wsaResult != 0) {
		std::cout << "WinSock2 failed to initialise" << std::endl;
		std::cout << "Error code: " << wsaResult << std::endl;
		return 1;
	}
	return 0;
}