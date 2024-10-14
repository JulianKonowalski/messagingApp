#include "msgBuffer.h"

void MsgBuffer::addMsg(std::string* message) {
	_mutex.lock();
	if (_messages.size() >= MAX_MESSAGES) {
		std::string* tmp = _messages.front();
		_messages.pop_front();
		delete tmp;
	}
	_messages.push_back(message);
	setChanged(true);
	_mutex.unlock();
	_notification.notify_all();
	return;
}