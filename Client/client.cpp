#include <SDL2/SDL.h>
#include <cstdint>
#include <map>
#include <iostream>
#include <utility>
#include "../Common/Socket.h"
#include "../Common/Action.h"
#include "../Common/Time.h"
#include "../Common/Tools.h"
#include "./SDL.h"
#include "./Keyboard.h"

std::map<uint32_t, uint8_t> inputQueue;
uint8_t player_x = 0;
uint8_t player_y = 0;
SDL_Renderer *renderer;

void receiveStatus(const Socket& s) {
	// 4(frame) + 4(#players) + 2(player)*#players
	Address server;
	uint8_t data[250];
	int bytes = s.receive(&server, data, 250);
	if (bytes < 6 or bytes%2 == 1) return;
	uint32_t frame = Tools::bytesToUint32(data, 0);
	int nPlayers = Tools::bytesToUint32(data, 4);
	uint8_t old_player_x = player_x;
	uint8_t old_player_y = player_y;
	player_x = data[8];
	player_y = data[9];
	auto it = inputQueue.upper_bound(frame);
	inputQueue.erase(inputQueue.begin(), it);
	for (it = inputQueue.begin(); it != inputQueue.end(); ++it) {
		Tools::runInput(&player_x, &player_y, it->second);
	}
	if (old_player_y != player_y or old_player_x != player_x) {
		std::cout << "Position corrected (" << old_player_x-player_x << "," << old_player_y-player_y << ") "
		<< "Input queue size: " << inputQueue.size() << std::endl;
		std::cout << "From->To: X:" << static_cast<unsigned int>(data[4]) << "->" << static_cast<unsigned int>(player_x)
		<< "\tY:" << static_cast<unsigned int>(data[5]) << "->" << static_cast<unsigned int>(player_y) << std::endl;
	}

	for (int i = 0; i < nPlayers-1; ++i) {
		SDL_RenderDrawPoint(renderer, data[10+2*i], data[11+2*i]);
  		SDL_RenderPresent(renderer);
	}
}

int main()
{
	try {
		bool isRunning = true;
		SDL_Window *window;
		SDL sdl(SDL_INIT_VIDEO|SDL_INIT_TIMER);
		SDL_CreateWindowAndRenderer(256, 256, 0, &window, &renderer);

		Socket socket;
		if (!socket.open(0))
			return -1;

		float deltaTime = 0;
		float lastFrameTime = 0;
		SDL_Event event;
		Keyboard keyboard = Keyboard(&event);
		uint32_t frame = 0;
		while (isRunning) {
			deltaTime += Time::getClock()-lastFrameTime;
			lastFrameTime = Time::getClock();
			receiveStatus(socket);
			if (deltaTime < 0.033) continue;
			keyboard.update();
			uint8_t data[5];
			Action::Action move = Action::NONE;
			Action::Action action = Action::NONE;
			if (keyboard.isKeyHeld(SDLK_UP)) move = Action::MOVE_UP;
			else if (keyboard.isKeyHeld(SDLK_RIGHT)) move = Action::MOVE_RIGHT;
			else if (keyboard.isKeyHeld(SDLK_LEFT)) move = Action::MOVE_LEFT;
			else if (keyboard.isKeyHeld(SDLK_DOWN)) move = Action::MOVE_DOWN;
			if (keyboard.isKeyPressed(SDLK_SPACE)) action = Action::ACTION;
			Tools::intToBytes(frame, data, 0);
			data[4] = Action::toEncode(move, (action != Action::NONE));
			if (socket.send(Address(188,226,170,23,30000), data, 5)) {
			// if (socket.send(Address(127,0,0,1,30000), data, 5)) {
				inputQueue.insert(std::pair<uint32_t, uint8_t>(frame, data[4]));
				Tools::runInput(&player_x, &player_y, data[4]);
			}
			SDL_RenderDrawPoint(renderer, player_x, player_y);
  			SDL_RenderPresent(renderer);
			frame++;
			deltaTime = 0;
			isRunning = !(keyboard.isKeyPressed(SDLK_ESCAPE) or (keyboard.isKeyHeld(SDLK_LCTRL) and keyboard.isKeyPressed(SDLK_q)));
		}
		SDL_DestroyWindow(window);
		return 0;
	}
	catch (const InitError& err) {
		std::cerr
			<< "Error while initializing SDL:  "
			<< err.what() << std::endl;
	}
	return 1;
}