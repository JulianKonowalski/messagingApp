#include "client.h"

#define DEFAULT_BUF_LEN 128

Client::Client(void) : _serverAddr(NULL), _serverPort(NULL), _socket(INVALID_SOCKET), _bufLen(DEFAULT_BUF_LEN) {
	_recvBuf = new char[_bufLen];
}

int Client::createSocket(PCSTR serverAddr, PCSTR serverPort) {
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

	return 0;
}

int Client::sendMsg(const char* msg) {

	if (_socket == INVALID_SOCKET) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Client is not connected to the server\n" << std::endl;
		return -1;
	}

	int msgLen = (int)strlen(msg) + 1; //+1 is for the \0 ending byte
	int wsaResult = send(_socket, (char*)&msgLen, sizeof(int), 0); //send msg size before sending the message
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}

	wsaResult = send(_socket, msg, msgLen, 0); //send msg
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to send the message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return -1;
	}

	return 0;
}

std::string Client::receiveMsg(void) {

	if (_socket == INVALID_SOCKET) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "No server connected\n" << std::endl;
		return std::string("");
	}

	int msgSize;
	int wsaResult = recv(_socket, (char*)&msgSize, sizeof(int), 0); //receive message size before the message
	if (wsaResult < 0) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		closesocket(_socket);
		_socket = INVALID_SOCKET;
		return std::string("");
	}

	wsaResult = recv(_socket, _recvBuf, _bufLen, 0); //receive message
	if (wsaResult < 0) {
		std::cout << "Failed to receive message" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		closesocket(_socket);
		_socket = INVALID_SOCKET;
		return std::string("");
	}

	return std::string(_recvBuf);

}