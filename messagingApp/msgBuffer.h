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
	bool isChanged(void) { return _changed; }
	bool isActive(void) { return _active; }
	const std::list<std::string*>* getMessages(void);
	void shutdown(void);

private:
	std::mutex _mutex;
	std::condition_variable _notification;
	bool _changed = false;
	bool _active = true;
	std::list<std::string*> _messages;
};

#endif
