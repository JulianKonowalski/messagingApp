#include "host.h"

int Host::createSocket(void) {

	struct addrinfo* addrResult = nullptr, info;

	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;
	info.ai_flags = AI_PASSIVE;

	int wsaResult = getaddrinfo(nullptr, _port, &info, &addrResult);
	if (wsaResult != 0) {
		std::cout << "getaddrinfo failed" << std::endl;
		std::cout << "Error code: " << wsaResult << "\n" << std::endl;
		return -1;
	}

	_hostSocket = INVALID_SOCKET;
	_hostSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (_hostSocket == INVALID_SOCKET) {
		std::cout << "Failed to initialise server socket" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		return -1;
	}

	wsaResult = bind(_hostSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to bind server socket to port " << _port << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		closesocket(_hostSocket);
		return -1;
	}

	freeaddrinfo(addrResult);

	return 0;
}

int Host::startListen(void) {
	if (listen(_hostSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}
	return 0;
}

int Host::connect (void) {
	_socket = accept(_hostSocket, nullptr, nullptr);
	if (_socket == INVALID_SOCKET) {
		std::cout << "Connention accept failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}
	_active = true;
	return 0;
}