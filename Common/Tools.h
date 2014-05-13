#ifndef TOOLS_HPP
#define TOOLS_HPP
#include <cstdint>
#include "./Action.h"

namespace Tools {
	void intToBytes(uint32_t i, uint8_t data[], int start);
	uint32_t bytesToUint32(uint8_t data[], int start);
	void runInput(uint8_t* x, uint8_t* y, uint8_t input);
}

#endif
