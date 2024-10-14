#include "app.h"

#include <thread>
#include <condition_variable>

#include "wsaInit.h"

#define DEFAULT_SERVER_PORT "8080"

enum userType {HOST, CLIENT};
enum userAction {HOST_ROOM, JOIN_ROOM};

void App::run(void) {
	try {
		setupWsa();
		welcomeMessage();
		connect();
		chatRoom();
	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl << std::flush;
		exit(1);
	}
}

void App::setupWsa(void) {
	if (initialiseWSA(MAKEWORD(2, 2)) != 0) { 
		throw std::runtime_error("Failed to initialise WSA");
	}
}

void App::welcomeMessage(void) {
	std::cout << "This is a welcome message!" << std::endl << std::flush;
}

void App::connect(void) {
	switch (getUserChoice()) {
	case HOST_ROOM:
		std::cout << "Waiting for a request..." << std::endl << std::flush;
		connectWithClient();
		break;
	case JOIN_ROOM:
		connectWithServer();
		break;
	}
	system("cls");
}

void App::connectWithClient(void) {
	if (_server.createSocket(DEFAULT_SERVER_PORT) != 0) { 
		throw std::runtime_error("Failed to initialise server socket"); 
	}
	_server.startListen();
	while (_server.connect() != 0);
}

void App::connectWithServer(void) {
	std::string serverAddr;
	do {
		std::cout << "Input the host address You wish to connect with: " << std::flush;
		std::cin >> serverAddr;
		system("cls");
	} while (_client.createSocket(serverAddr.c_str(), DEFAULT_SERVER_PORT) != 0);
}

void App::chatRoom(void) {
	std::thread printThr(&App::printMessages, this);
	std::thread recvThr(&App::receiveMessages, this);
	std::thread sendThr(&App::sendMessages, this);
	printThr.join();
	recvThr.join();
	sendThr.join();
}

void App::printMessages(void) {
	std::unique_lock<std::mutex> lock(*_buffer.getMutex());
	std::condition_variable* notification = _buffer.getCv();
	while (true) {
		notification->wait(lock, [this] { return _buffer.changed(); });
		const std::list<std::string*>* messages = _buffer.getMessages();
		system("cls");
		for (auto const& message : *messages) {
			std::cout << *message << std::endl << std::flush;
		}
		_buffer.setChanged(false);
	}
}

void App::receiveMessages(void) {
	switch (_userType) {
	case HOST:
		receiveAsHost();
		break;
	case CLIENT:
		receiveAsClient();
		break;
	}
}

void App::receiveAsHost(void) {
	while (true) {
		std::string* message = new std::string(_server.receiveMsg());
		_buffer.addMsg(message);
	}
}

void App::receiveAsClient(void) {
	while (true) {
		std::string* message = new std::string(_client.receiveMsg());
		_buffer.addMsg(message);
	}
}

void App::sendMessages(void) {
	switch (_userType) {
	case HOST:
		sendAsHost();
		break;
	case CLIENT:
		sendAsClient();
		break;
	}
}

void App::sendAsHost(void) {
	while (true) {
		std::string* message = new std::string(getMessage());
		_buffer.addMsg(message);
		_server.sendMsg(message->c_str());
	}
}

void App::sendAsClient(void) {
	while (true) {
		std::string* message = new std::string(getMessage());
		_buffer.addMsg(message);
		_client.sendMsg(message->c_str());
	}
}

std::string App::getMessage(void) {
	std::string message;
	do {
		std::getline(std::cin, message);
	} while (message == "" || message == "\n");
	return message;
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
		_userType = HOST;
		return HOST_ROOM;
	}
	_userType = CLIENT;
	return JOIN_ROOM;
}