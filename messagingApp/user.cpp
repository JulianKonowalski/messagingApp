#include "user.h"

#define DEFAULT_BUF_LEN 128

#include <string>

User::User(void) : _socket(INVALID_SOCKET), _bufLen(DEFAULT_BUF_LEN) {
	_recvBuf = new char[_bufLen];
}

std::string User::receiveMsg(void) {

	if (_socket == INVALID_SOCKET) {
		throw std::runtime_error("Failed to receive the message. Socket is not connected");
	}

	int msgSize;
	int wsaResult = recv(_socket, (char*)&msgSize, sizeof(int), 0); //receive message size before the message
	if (wsaResult < 0) {
		std::string errorMsg("Failed to receive the message size. Socket error ");
		errorMsg += std::to_string(WSAGetLastError());
		throw std::runtime_error(errorMsg);
	}

	wsaResult = recv(_socket, _recvBuf, _bufLen, 0); //receive message
	if (wsaResult < 0) {
		std::string errorMsg("Failed to receive the message. Socket error ");
		errorMsg += std::to_string(WSAGetLastError());
		throw std::runtime_error(errorMsg);
	}

	return std::string(_recvBuf);
}

void User::sendMsg(const char* msg) {

	if (_socket == INVALID_SOCKET) {
		throw std::runtime_error("Failed to send the message. Socket is not connected");
	}

	int msgLen = (int)strlen(msg) + 1; //+1 is for the \0 ending byte
	int wsaResult = send(_socket, (char*)&msgLen, sizeof(int), 0); //send msg size before sending the message
	if (wsaResult == SOCKET_ERROR) {
		std::string errorMsg("Failed to send the message size. Socket error ");
		errorMsg += std::to_string(WSAGetLastError());
		throw std::runtime_error(errorMsg);
	}

	wsaResult = send(_socket, msg, msgLen, 0); //send msg
	if (wsaResult == SOCKET_ERROR) {
		std::string errorMsg("Failed to send the message. Socket error ");
		errorMsg += std::to_string(WSAGetLastError());
		throw std::runtime_error(errorMsg);
	}

}