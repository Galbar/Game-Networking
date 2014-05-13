#ifndef ACTION_HPP
#define ACTION_HPP
#include <cstdint>
#include <string>

namespace Action
{
	enum Action
	{
		NONE,           // 0
		MOVE_UP,        // 1
		MOVE_DOWN,      // 2
		MOVE_LEFT,      // 3
		MOVE_RIGHT,     // 4
		ACTION,         // 0
		SIZE
	};

	std::string toString(Action a);
	Action getMove(uint8_t c);
	bool isAction(uint8_t c);
	uint8_t toEncode(Action movement, bool action);
}
#endif
