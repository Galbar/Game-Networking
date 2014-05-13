#include "./Frame.h"

Frame::Frame() {
	loopInput = std::list<Input>();
}

Frame::~Frame() {}

void Frame::pushInput(float dt, std::set<SDL_Keycode> input) {
	uint32_t deltaTime = dt*1000;
	Input i{deltaTime, input};
	loopInput.push_back(i);
}

uint32_t Frame::getSizeInBytes() const {
	uint32_t packetSize = 4 + loopInput.size()*8; // SIZE_TOTAL + inputs(time+size), faltan los sets
	for (std::list<Input>::const_iterator input = loopInput.cbegin(); input != loopInput.cend(); ++input) {
		packetSize += 4*input->input.size();
	}
	return packetSize;
}

void Frame::getAsBytes(uint8_t *data, uint32_t size) const {
	// (data)[ SIZE_TOTAL | (loops)[ (inputs)[ TIME | SIZE | [ INPUT_VALUE | ... ] | ... ] | ... ] ]
	data[0] = size;
	int its1 = 0;
	for (std::list<Input>::const_iterator input = loopInput.cbegin(); input != loopInput.cend(); ++input) {
		data[4+its1*8] = input->miliseconds;
		data[8+its1*8] = input->input.size();
		int its2 = 12+its1*8;
		for (std::set<SDL_Keycode>::const_iterator key = input->input.cbegin(); key != input->input.cend(); ++key) {
			data[its2] = (uint32_t) *key;
			its2 += 4;
		}
		its1++;
	}
}