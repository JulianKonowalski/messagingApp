#include "app.h"

#include <thread>
#include <condition_variable>

#include "wsaInit.h"
#include "host.h"
#include "user.h"

const char* DEFAULT_SERVER_PORT = "8080";
const std::string QUIT_MSG("\\q");

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
	delete(_user);
	WSACleanup();
}

void App::welcomeMessage(void) {
	gui.printLine("Welcome to Messaging App!");
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
	gui.printLine("Waiting for a request...");
	tmpUser->startListen();
	while (tmpUser->connect() != 0);
	gui.clear();
	_user = tmpUser;
}

void App::joinRoom(void) {
	Client* tmpUser = nullptr;
	std::string serverAddr;
	do {
		delete(tmpUser);
		gui.printLine("Input the host address You wish to connect with: ", 0);
		std::cin >> serverAddr;
		tmpUser = new Client(serverAddr.c_str(), DEFAULT_SERVER_PORT);
		gui.clear();
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
		notification->wait(lock, [this] { return _buffer.isChanged(); });
		if (!_buffer.isActive()) {
			break;
		}
		gui.clear();
		gui.printMessages(_buffer.getMessages());
		_buffer.setChanged(false);
	}
}

void App::receiveMessages(void) {
	while (true) {
		std::string* message = new std::string(_user->receiveMsg());
		if (*message == SHUTDOWN_REQUEST_FLAG) {
			gui.clear();
			gui.printLine("Other user left the chat room");
			gui.printLine("Type anything to leave");
			break; 
		} else if (*message == SHUTDOWN_CONFIRMATION_FLAG) {
			break;
		}
		_buffer.addMsg(message);
	}
	_buffer.shutdown();
	_user->shutdown();
}

void App::sendMessages(void) {
	while (true) {
		std::string* message = new std::string(getUserMessage());
		if (*message == QUIT_MSG) {
			gui.clear();
			gui.printLine("You left the chat room");
			_user->shutdown();
			break;
		} else if (!_user->isActive()) {
			break;
		}
		_buffer.addMsg(message);
		_user->sendMsg(message->c_str());
	}
}

std::string App::getUserMessage(void) {
	std::string message;
	do {
		std::getline(std::cin, message);
	} while (message == "" || message == "\n");
	return message;
}

int App::getUserChoice(void) {
	std::string choice;
	do {
		gui.printLine("Do you wish to host a chat room or join an existing one?");
		gui.printLine("host/join");
		std::cin >> choice;
		gui.clear();
	} while (choice != "host" && choice != "join");
	if (choice == "host") {
		return HOST_ROOM;
	}
	return JOIN_ROOM;
}