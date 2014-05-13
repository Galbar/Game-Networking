#include "./Keyboard.h"

Keyboard::Keyboard(SDL_Event* event) {
	this->event = event;
	keysHeld = std::set<SDL_Keycode>();
	keysHeldOld = std::set<SDL_Keycode>();
}

Keyboard::~Keyboard() {}

void Keyboard::update() {
	keysHeldOld = keysHeld;
	while (SDL_PollEvent(event)) {
		if (event->type == SDL_KEYDOWN) {
			keysHeld.insert(event->key.keysym.sym);
		}
		else if (event->type == SDL_KEYUP) {
			keysHeld.erase(event->key.keysym.sym);
		}
	}
}

bool Keyboard::isKeyPressed(SDL_Keycode key) {
	return (keysHeld.find(key) != keysHeld.end() and keysHeldOld.find(key) == keysHeldOld.end());
}

bool Keyboard::isKeyReleased(SDL_Keycode key) {
	return (keysHeld.find(key) == keysHeld.end() and keysHeldOld.find(key) != keysHeldOld.end());
}

bool Keyboard::isKeyHeld(SDL_Keycode key) {
	return (keysHeld.find(key) != keysHeld.end() and keysHeldOld.find(key) != keysHeldOld.end());
}

std::set<SDL_Keycode> Keyboard::getPressedKeys() {
	std::set<SDL_Keycode> ret = std::set<SDL_Keycode>();
	for (std::set<SDL_Keycode>::iterator it=keysHeld.begin(); it!=keysHeld.end(); ++it) {
		if (keysHeldOld.find(*it) == keysHeldOld.end()) {
			ret.insert(*it);
		}
	}
	return ret;
}
std::set<SDL_Keycode> Keyboard::getHeldKeys() {
	std::set<SDL_Keycode> ret = std::set<SDL_Keycode>();
	for (std::set<SDL_Keycode>::iterator it=keysHeld.begin(); it!=keysHeld.end(); ++it) {
		if (keysHeldOld.find(*it) != keysHeldOld.end()) {
			ret.insert(*it);
		}
	}
	return ret;
}
std::set<SDL_Keycode> Keyboard::getReleasedKeys() {
	std::set<SDL_Keycode> ret = std::set<SDL_Keycode>();
	for (std::set<SDL_Keycode>::iterator it=keysHeldOld.begin(); it!=keysHeldOld.end(); ++it) {
		if (keysHeld.find(*it) == keysHeld.end()) {
			ret.insert(*it);
		}
	}
	return ret;
}