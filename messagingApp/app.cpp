#include "app.h"

#include <thread>
#include <condition_variable>

#include "wsaInit.h"
#include "host.h"
#include "user.h"

#define DEFAULT_SERVER_PORT "8080"

enum userAction {HOST_ROOM, JOIN_ROOM};

void App::run(void) {
	try {
		initialiseWSA(MAKEWORD(2, 2));
		welcomeMessage();
		connect();
		chatRoom();
	} catch (std::runtime_error& error) {
		std::cout << error.what() << std::endl << std::flush;
		exit(1);
	}
}

void App::welcomeMessage(void) {
	std::cout << "Welcome to Messaging App!" << std::endl << std::flush;
}

void App::connect(void) {
	switch (getUserChoice()) {
	case HOST_ROOM:
		hostRoom();
		break;
	case JOIN_ROOM:
		joinRoom();
		break;
	}
}

void App::hostRoom(void) {
	Host* tmpUser = new Host(DEFAULT_SERVER_PORT);
	if (tmpUser->createSocket() != 0) {
		throw std::runtime_error("Failed to initialise host socket"); 
	}
	std::cout << "Waiting for a request..." << std::endl << std::flush;
	tmpUser->startListen();
	while (tmpUser->connect() != 0);
	system("cls");
	_user = tmpUser;
}

void App::joinRoom(void) {
	Client* tmpUser = nullptr;
	std::string serverAddr;
	do {
		delete(tmpUser);
		std::cout << "Input the host address You wish to connect with: " << std::flush;
		std::cin >> serverAddr;
		tmpUser = new Client(serverAddr.c_str(), DEFAULT_SERVER_PORT);
		system("cls");
	} while (tmpUser->createSocket() != 0);
	_user = tmpUser;
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
	while (true) {
		std::string* message = new std::string(_user->receiveMsg());
		_buffer.addMsg(message);
	}
}

void App::sendMessages(void) {
	while (true) {
		std::string* message = new std::string(getMessage());
		_buffer.addMsg(message);
		_user->sendMsg(message->c_str());
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
		return HOST_ROOM;
	}
	return JOIN_ROOM;
}