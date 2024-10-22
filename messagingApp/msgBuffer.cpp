#include "msgBuffer.h"

MsgBuffer::~MsgBuffer(void) {
	while (_messages.size() != 0) {
		delete _messages.back();
		_messages.pop_back();
	}
}

void MsgBuffer::addMsg(Message* message) {
	_mutex.lock();
	if (_messages.size() >= MAX_MESSAGES) {
		delete _messages.front();
		_messages.pop_front();
	}
	_messages.push_back(message);
	_changed = true;
	_mutex.unlock();
	_notification.notify_all();
	return;
}
const std::list<Message*>* MsgBuffer::getMessages(void) {
	return &_messages;
}

void MsgBuffer::shutdown(void) {
	_mutex.lock();
	_active = false;
	_changed = true;
	_mutex.unlock();
	_notification.notify_all();
}