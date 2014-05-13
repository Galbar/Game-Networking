#include <SDL2/SDL.h>
#include <cstdint>
#include <iostream>
#include <string>
#include "../Common/Socket.h"
//#include "../Common/Action.h"
#include "../Common/Time.h"
#include "../Common/Tools.h"
#include "./PlayerHandler.h"

Socket sock;

int main()
{
	if (!sock.open(30000))
		return -1;
	float timeSinceStatusUpdate = 0;
	float lastFrameTime = 0;
	PlayerHandler playerHandler(1);
	while (true) {
		timeSinceStatusUpdate += Time::getClock()-lastFrameTime;
		lastFrameTime = Time::getClock();
		playerHandler.removeExpiredConnections();
		if (timeSinceStatusUpdate > 0.1) {
			for (PlayerHandler::iterator it = playerHandler.begin(); it != playerHandler.end(); ++it) {
				// 4(frame) + 4(#players) + 2(player)*#players
				int dataSize = 8+2*playerHandler.size();
				uint8_t data[dataSize];
				Tools::intToBytes(it.player->frame, data, 0);
				Tools::intToBytes(playerHandler.size(), data, 4);
				data[8] = it.player->x;
				data[9] = it.player->y;
				int i = 10;
				for (PlayerHandler::iterator k = playerHandler.begin(); k != playerHandler.end(); ++k) {
					if (k == it) continue;
					data[i] = k.player->x;
					data[i+1] = k.player->y;
					std::cout << " Player: " << "\tx: " << (int)k.player->x << "\ty: " << (int)k.player->y << std::endl;
					std::cout << "~Player: " << "\tx: " << (int)data[i] << "\ty: " << (int)data[i+1] << std::endl;
					i += 2;
				}
				sock.send(*(it.address), data, dataSize);
			}
			timeSinceStatusUpdate = 0;
		}
		Address sender;
		uint8_t data[5];
		int n_bytes = sock.receive(&sender, data, 5);
		if (n_bytes < 5) continue;
		uint32_t frame = Tools::bytesToUint32(data, 0);
		if (playerHandler.isNewConnection(sender))
			std::cout << "Jugador nuevo conectado: " << sender.getAddressAsString() << std::endl;
		playerHandler.updateConnection(sender);
		Player& player = playerHandler.getPlayer(sender);
		if (frame > player.frame) {
			if (player.frame != frame-1)
				std::cout << "Se han perdido " << frame-player.frame << " paquetes" << std::endl;
			Tools::runInput(&player.x, &player.y, data[4]);
			player.frame = frame;
		} else if (frame == player.frame){
			std::cout << "Paquete duplicado" << std::endl;
		} else {
			std::cout << "Paquete retrasado (" << frame << "/" << player.frame << ")" << std::endl;
		}
	}
	return 0;
}