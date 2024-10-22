#include "gui.h"

#include <windows.h>

void GUI::jump(int x, int y) {
	if (x < 0 || y < 0) {
		throw std::runtime_error("Invalid console cursor coordinates");
	}
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), {(SHORT)x, (SHORT)y});
}

void GUI::printLine(std::string message, int ending) {
	switch (ending) {
	case 0:
		std::cout << message << std::flush;
		break;
	case 1:
		std::cout << message << std::endl << std::flush;
		break;
	}
}

void GUI::printMessages(const std::list<Message*>* messages) {
	for (auto const& message : *messages) {
		std::cout << *message << std::endl << std::flush;
	}
}