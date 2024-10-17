#ifndef GUI_H
#define GUI_H

#include <iostream>
#include <list>

class GUI {
public:
	GUI(void) {}
	~GUI(void) {}
	
	void clear(void) { system("cls"); }
	void jump(int x, int y);
	void printLine(const std::string message, int ending = 1);
	void printMessages(const std::list<std::string*>* messages);

};

#endif