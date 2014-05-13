#ifndef FRAME_H
#define FRAME_H
#include <SDL2/SDL.h>
#include <cstdint>
#include <set>
#include <list>
class Frame
{
public:
	Frame();
	~Frame();
	void pushInput(float dt, std::set<SDL_Keycode> input);
	void getAsBytes(uint8_t *data, uint32_t size) const;
	uint32_t getSizeInBytes() const;
private:
	struct Input
	{
		uint32_t miliseconds;
		std::set<SDL_Keycode> input;
	};
	std::list<Input> loopInput;
};
#endif