#include "./Player.h"

Player::Player() {
	x = y = frame =0;
}

Player::~Player() {

}

void Player::toEncode(uint8_t data[], int start) {
	Tools::intToBytes(frame, data, 0);
	data[start+4] = x;
	data[start+5] = y;
}
