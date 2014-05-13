#include "./Time.h"

float Time::getClock() {
	return 0.001f*SDL_GetTicks();
}