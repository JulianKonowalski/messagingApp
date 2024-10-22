#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include <iostream>
#include <list>
#include <string>
#include <mutex>

#define MAX_MESSAGES 5

struct Message {
	Message(std::string author, std::string message) : _author(author), _message(message) {}
	const std::string _author;
	const std::string _message;
};
inline std::ostream& operator<<(std::ostream& stream, const Message& msg) {
	return stream << msg._author << ": " << msg._message;
}

class MsgBuffer {
public:
	~MsgBuffer(void);
	void addMsg(Message* message);
	std::mutex* getMutex(void) { return &_mutex; }
	std::condition_variable* getCv(void) { return &_notification; }
	bool isChanged(void) { return _changed; }
	void setChanged(bool changed) { _changed = changed; }
	bool isActive(void) { return _active; }
	const std::list<Message*>* getMessages(void);
	void shutdown(void);

private:
	std::mutex _mutex;
	std::condition_variable _notification;
	bool _changed = false;
	bool _active = true;
	std::list<Message*> _messages;
};

#endif
