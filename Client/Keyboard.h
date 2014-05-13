#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <SDL2/SDL.h>
#include <set>

class Keyboard
{
public:
	explicit Keyboard(SDL_Event* event);
	~Keyboard();
	void update();
	bool isKeyPressed(SDL_Keycode key);
	bool isKeyReleased(SDL_Keycode key);
	bool isKeyHeld(SDL_Keycode key);
	std::set<SDL_Keycode> getPressedKeys();
	std::set<SDL_Keycode> getHeldKeys();
	std::set<SDL_Keycode> getReleasedKeys();
private:
	std::set<SDL_Keycode> keysHeld;
	std::set<SDL_Keycode> keysHeldOld;
	SDL_Event* event;
};

#endif
