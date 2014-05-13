#include "./Action.h"

std::string Action::toString(Action a) {
	if (a == NONE)
		return "none";

	if (a == MOVE_UP)
		return "move up";

	if (a == MOVE_DOWN)
		return "move down";

	if (a == MOVE_LEFT)
		return "move left";

	if (a == MOVE_RIGHT)
		return "move right";

	if (a == ACTION)
		return "action";

	if (a == SIZE)
		return "size";

	return "nope";
}

Action::Action Action::getMove(uint8_t c) {
	return (Action) (c & 0x1F);
}

bool Action::isAction(uint8_t c) {
	return (c & 0x20);
}

uint8_t Action::toEncode(Action movement, bool action) {
	uint8_t r = 0;
	if (action) r = 1 << 5;
	r |= movement;
	return r;
}