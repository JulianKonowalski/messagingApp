#include "client.h"

#include <iostream>

int Client::createSocket(PCSTR serverAddr, PCSTR serverPort) {
	std::cout << "Initialising Client Socket" << std::endl;

	_serverAddr = serverAddr;
	_serverPort = serverPort;
	struct addrinfo* addrResult = nullptr, info;

	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;

	int wsaResult = getaddrinfo(_serverAddr, _serverPort, &info, &addrResult);
	if (wsaResult != 0) {
		std::cout << "getaddrinfo failed" << std::endl;
		std::cout << "Error code: " << wsaResult << "\n" << std::endl;
		return -1;
	}

	_socket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (_socket == INVALID_SOCKET) {
		std::cout << "Failed to initialise client socket" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		return -1;
	}

	wsaResult = connect(_socket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Client socket error" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		closesocket(_socket);
		_socket = INVALID_SOCKET;
		return -1;
	}

	if (_socket == INVALID_SOCKET) {
		std::cout << "Failed to connect to server" << std::endl;
		return -1;
	}

	freeaddrinfo(addrResult);
	std::cout << "Client socket initialised successfully\n" << std::endl;

	return 0;
}

int Client::sendMsg(const char* msg) {

	if (_socket == INVALID_SOCKET) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Client is not connected to the server\n" << std::endl;
		return 1;
	}

	int wsaResult = send(_socket, msg, (int)strlen(msg), 0);
	if (wsaResult != 0) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
	}

	std::cout << "Message sent\n" << std::endl;

	return 0;
}