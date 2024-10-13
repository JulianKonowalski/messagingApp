#include "app.h"

#include <thread>

#include "wsaInit.h"

#define DEFAULT_SERVER_PORT "8080"
#define HOST_ROOM 1
#define JOIN_ROOM 2

void App::run(void) {
	try {
		setupWsa();
		connect();
	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl << std::flush;
		exit(1);
	}
	chatRoom();
}

void App::setupWsa(void) {
	if (initialiseWSA(MAKEWORD(2, 2)) != 0) { 
		throw std::runtime_error("Failed to initialise WSA");
	}
}

void App::connect(void) {
	switch (getUserChoice()) {
	case HOST_ROOM:
		awaitConnection();
		break;
	case JOIN_ROOM:
		connectWithServer();
		break;
	}
	std::cout << "Connected" << std::endl << std::flush;
}

void App::awaitConnection(void) {
	if (_server.createSocket(DEFAULT_SERVER_PORT) != 0) { 
		throw std::runtime_error("Failed to initialise server socket"); 
	}
	_server.startListen();
	while (_server.connect() != 0);
}

void App::connectWithServer(void) {
	std::string serverAddr;
	do {
		std::cout << "Input the address You wish to connect with: " << std::flush;
		std::cin >> serverAddr;
		system("cls");
	} while (_client.createSocket(serverAddr.c_str(), DEFAULT_SERVER_PORT) != 0);
}

void App::chatRoom(void) {

}

int App::getUserChoice(void) {
	std::string choice;
	do {
		std::cout << "Do you wish to host a chat room or join an existing one?" << std::endl << std::flush;
		std::cout << "host/join" << std::endl << std::flush;
		std::cin >> choice;
		system("cls");
	} while (choice != "host" && choice != "join");
	if (choice == "host") {
		return HOST_ROOM;
	}
	return JOIN_ROOM;
}