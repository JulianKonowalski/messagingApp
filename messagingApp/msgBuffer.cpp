#include "msgBuffer.h"

MsgBuffer::~MsgBuffer(void) {
	while (_messages.size() != 0) {
		delete _messages.back();
		_messages.pop_back();
	}
}

void MsgBuffer::addMsg(std::string* message) {
	_mutex.lock();
	if (_messages.size() >= MAX_MESSAGES) {
		std::string* tmp = _messages.front();
		_messages.pop_front();
		delete tmp;
	}
	_messages.push_back(message);
	_changed = true;
	_mutex.unlock();
	_notification.notify_all();
	return;
}

void MsgBuffer::shutdown(void) {
	_mutex.lock();
	_active = false;
	_changed = true;
	_mutex.unlock();
	_notification.notify_all();
}