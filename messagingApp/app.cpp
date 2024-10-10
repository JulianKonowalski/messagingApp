#include "app.h"

#include <thread>

#include "wsaInit.h"

#define DEFAULT_SERVER_PORT "8080"

void App::run(void) {
	if (setupWsa() != 0) { exit(1); }
	connect();
	std::cout << "connected";
}

int App::setupWsa(void) {
	if (initialiseWSA(MAKEWORD(2, 2)) != 0) { return -1; }
	if (_server.createSocket(DEFAULT_SERVER_PORT) != 0) { return -1; }
	return 0;
}

void App::connect(void) {
	bool connected = false;
	std::mutex connectedMutex;
	std::thread serverThread(&App::awaitConnection, this, &connected, &connectedMutex);
	std::thread clientThreat(&App::connectWithServer, this, &connected, &connectedMutex);
	while (!connected);
}

void App::awaitConnection(bool* connected, std::mutex* connectedMutex) {
	_server.startListen();
	while (_server.connect() != 0);
	connectedMutex->lock();
	*connected = true;
	connectedMutex->unlock();
}

void App::connectWithServer(bool* connected, std::mutex* connectedMutex) {
	std::string serverAddr;
	do {
		serverAddr = getServerAddr();
	} while (_client.createSocket(serverAddr.c_str(), DEFAULT_SERVER_PORT) != 0);
	connectedMutex->lock();
	*connected = true;
	connectedMutex->unlock();
}

std::string App::getServerAddr(void) {
	std::cout << "Input the address You wish to connect with: ";
	std::string serverAddr;
	std::cin >> serverAddr;
	return serverAddr;
}