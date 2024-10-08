#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib,"WS2_32")

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

/* SERVER SIDE */

SOCKET createServerSocket(PCSTR port) {
	std::cout << "Initialising Server Socket" << std::endl;

	struct addrinfo *addrResult = nullptr, info;

	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;
	info.ai_flags = AI_PASSIVE;

	int wsaResult = getaddrinfo(nullptr, port, &info, &addrResult);
	if (wsaResult != 0) {
		std::cout << "getaddrinfo failed" << std::endl;
		std::cout << "Error code: " << wsaResult << "\n" << std::endl;
		return INVALID_SOCKET;
	}

	SOCKET serverSocket = INVALID_SOCKET;
	serverSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Failed to initialise server socket" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		return INVALID_SOCKET;
	}

	wsaResult = bind(serverSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Failed to bind server socket to port " << port << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		closesocket(serverSocket);
		return INVALID_SOCKET;
	}

	freeaddrinfo(addrResult);
	std::cout << "Server socket initialised successfully\n" << std::endl;

	return serverSocket;
}

int serverListen(SOCKET serverSocket) {
	std::cout << "Initialising listening" << std::endl;
	if (listen(serverSocket, SOMAXCONN) == SOCKET_ERROR) {
		std::cout << "Listen failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return 1;
	}
	std::cout << "Listening..." << std::endl;
	return 0;
}

int serverAccept(SOCKET serverSocket) {
	std::cout << "Accepting connection..." << std::endl;

	SOCKET clientSocket = INVALID_SOCKET;

	clientSocket = accept(serverSocket, nullptr, nullptr);
	if (clientSocket == INVALID_SOCKET){
		std::cout << "Connention accept failed" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		return INVALID_SOCKET;
	}

	std::cout << "Connection accepted\n" << std::endl;
	return clientSocket;
}

int serverSide(void) {

	if (initialiseWSA(MAKEWORD(2, 2)) != 0) {
		WSACleanup();
		return 1;
	}

	SOCKET serverSocket = createServerSocket("8080");
	if (serverSocket == INVALID_SOCKET) {
		WSACleanup();
		return 1;
	}

	while (true) {

		if (serverListen(serverSocket) != 0) {
			closesocket(serverSocket);
			WSACleanup();
			return 1;
		}

		SOCKET clientSocket = serverAccept(serverSocket);
		if (clientSocket == INVALID_SOCKET) {
			continue;
		}

	}

	WSACleanup();
	return 0;
}

/* CLIENT SIDE */

SOCKET createClientSocket(PCSTR serverAddr, PCSTR port) {
	std::cout << "Initialising Client Socket" << std::endl;

	struct addrinfo* addrResult = nullptr, info;

	ZeroMemory(&info, sizeof(info));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	info.ai_protocol = IPPROTO_TCP;

	int wsaResult = getaddrinfo(serverAddr, port, &info, &addrResult);
	if (wsaResult != 0) {
		std::cout << "getaddrinfo failed" << std::endl;
		std::cout << "Error code: " << wsaResult << "\n" << std::endl;
		return INVALID_SOCKET;
	}

	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Failed to initialise client socket" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		freeaddrinfo(addrResult);
		return INVALID_SOCKET;
	}

	wsaResult = connect(clientSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (wsaResult == SOCKET_ERROR) {
		std::cout << "Client socket error" << std::endl;
		std::cout << "Error code: " << WSAGetLastError() << "\n" << std::endl;
		closesocket(clientSocket);
		return INVALID_SOCKET;
	}

	if (clientSocket == INVALID_SOCKET) {
		std::cout << "Failed to connect to server" << std::endl;
		return INVALID_SOCKET;
	}

	freeaddrinfo(addrResult);
	std::cout << "Client socket initialised successfully\n" << std::endl;

	return clientSocket;
}

int clientSide(void) {

	if (initialiseWSA(MAKEWORD(2, 2)) != 0) {
		WSACleanup();
		return 1;
	}

	SOCKET clientSocket = createClientSocket("192.168.1.246", "8080");
	if (clientSocket == INVALID_SOCKET) {
		WSACleanup();
		return 1;
	}

	while (true) {

	}

	WSACleanup();
	return 0;
}