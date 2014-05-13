#include <cstdint>
#include "../Common/Tools.h"

class Player
{
public:
	Player();
	~Player();
	uint8_t x;
	uint8_t y;
	uint32_t frame;

	void toEncode(uint8_t data[], int start);
};