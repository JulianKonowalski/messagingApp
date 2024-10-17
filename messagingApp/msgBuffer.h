#ifndef MSGBUFFER_H
#define MSGBUFFER_H

#include <list>
#include <string>
#include <mutex>

#define MAX_MESSAGES 5

class MsgBuffer {
public:
	~MsgBuffer(void);
	void addMsg(std::string* message);
	std::mutex* getMutex(void) { return &_mutex; }
	std::condition_variable* getCv(void) { return &_notification; }
	bool changed(void) { return _changed; }
	void setChanged(bool changed) { _changed = changed; }
	const std::list<std::string*>* getMessages(void) { return &_messages; }

private:
	std::mutex _mutex;
	std::condition_variable _notification;
	bool _changed = false;
	std::list<std::string*> _messages;
};

#endif
