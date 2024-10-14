#include "server.h"

#define DEFAULT_BUF_LEN 128

Server::Server(void) : _port(NULL), _bufLen(DEFAULT_BUF_LEN) {
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

int Server::sendMsg(const char* msg) {

	if (_clientSocket == INVALID_SOCKET) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Server is not connected to the client\n" << std::endl;
		return -1;
	}

	int msgLen = (int)strlen(msg) + 1; //+1 is for the \0 ending byte
	int wsaResult = send(_clientSocket, (char*)&msgLen, sizeof(int), 0); //send msg size before sending the message
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}

	wsaResult = send(_clientSocket, msg, msgLen, 0); //send msg
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}

	return 0;
}

std::string Server::receiveMsg(void) {

	if (_clientSocket == INVALID_SOCKET) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "No client connected\n" << std::endl;
		return std::string("");
	}

	int msgSize;
	int wsaResult = recv(_clientSocket, (char*)&msgSize, sizeof(int), 0); //receive message size before the message
	if (wsaResult < 0) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		closesocket(_clientSocket);
		_clientSocket = INVALID_SOCKET;
		return std::string("");
	}

	wsaResult = recv(_clientSocket, _recvBuf, _bufLen, 0); //receive message
	if (wsaResult < 0) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		closesocket(_clientSocket);
		_clientSocket = INVALID_SOCKET;
		return std::string("");
	}

	return std::string(_recvBuf);
}