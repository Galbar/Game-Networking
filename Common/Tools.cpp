#include "./Tools.h"

void Tools::intToBytes(uint32_t i, uint8_t data[], int start) {
	data[start+0] = static_cast<uint8_t>((i & 0xFF000000) >> 24);
	data[start+1] = static_cast<uint8_t>((i & 0x00FF0000) >> 16);
	data[start+2] = static_cast<uint8_t>((i & 0x0000FF00) >> 8);
	data[start+3] = static_cast<uint8_t>(i & 0x000000FF);	
}

uint32_t Tools::bytesToUint32(uint8_t data[], int start) {
	return data[start+0] << 24 | data[start+1] << 16 | data[start+2] << 8 | data[start+3];
}

void Tools::runInput(uint8_t* x, uint8_t* y, uint8_t input) {
	Action::Action move = Action::getMove(input);
	/*Action::Action action = Action::NONE;
	if (Action::isAction(input))
		action = Action::ACTION;*/
	if (move == Action::MOVE_UP)
		(*y)--;
	else if (move == Action::MOVE_RIGHT)
		(*x)++;
	else if (move == Action::MOVE_LEFT)
		(*x)--;
	else if (move == Action::MOVE_DOWN)
		(*y)++;
}