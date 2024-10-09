#include "server.h"
#include "wsaInit.h"

#include <iostream>

Server::Server(PCSTR port, int bufLen) : _port(port), _bufLen(bufLen) {
	_recvBuf = new char[bufLen];
}

int Server::createSocket(void) {
	std::cout << "Initialising Server Socket" << std::endl;

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

	_socket = INVALID_SOCKET;
	_socket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (_socket == INVALID_SOCKET) {
		std::cout << "Failed to initialise server socket" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		return -1;
	}

	wsaResult = bind(_socket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to bind server socket to port " << _port << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		closesocket(_socket);
		return -1;
	}

	freeaddrinfo(addrResult);
	std::cout << "Server socket initialised successfully\n" << std::endl;

	return 0;
}

int Server::startListen(void) {
	std::cout << "Initialising listening" << std::endl;
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}
	std::cout << "Listening...\n" << std::endl;
	return 0;
}

int Server::acceptConnection(void) {
	_clientSocket = accept(_socket, nullptr, nullptr);
	std::cout << "Accepting connection..." << std::endl;
	if (_clientSocket == INVALID_SOCKET) {
		std::cout << "Connention accept failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}
	std::cout << "Connection accepted\n" << std::endl;
	return 0;
}

int Server::reveiveMsg(void) {

	if (_clientSocket == INVALID_SOCKET) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "No client connected\n" << std::endl;
		return -1;
	}

	int wsaResult;

	do {

		wsaResult = recv(_clientSocket, _recvBuf, _bufLen, 0);
		if (wsaResult > 0) {
			std::cout << "Received " << wsaResult << "bytes" << std::endl;
		}
		else if (wsaResult == 0) {
			std::cout << "Closing connection...\n" << std::endl;
		}
		else {
			std::cout << "Failed to receive message" << std::endl;
			std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
			closesocket(_clientSocket);
			_clientSocket = INVALID_SOCKET;
			return -1;
		}

	} while (wsaResult > 0);
	return 0;
}