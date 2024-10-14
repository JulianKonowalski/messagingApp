#include "server.h"

#include <iostream>

#define DEFAULT_BUF_LEN 512

Server::Server(void) : _port(NULL), _bufLen(DEFAULT_BUF_LEN) {
	_recvBuf = new char[_bufLen];
}

Server::Server(int bufLen) : _port(NULL), _bufLen(bufLen) {
	_recvBuf = new char[_bufLen];
}

int Server::createSocket(PCSTR port) {

	_port = port;
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

	return 0;
}

int Server::startListen(void) {
	if (listen(_socket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}
	return 0;
}

int Server::connect (void) {
	_clientSocket = accept(_socket, nullptr, nullptr);
	if (_clientSocket == INVALID_SOCKET) {
		std::cout << "Connention accept failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}
	return 0;
}

int Server::receiveMsg(void) {

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