#include <utility>
#include <iostream>
#include "./PlayerHandler.h"

PlayerHandler::PlayerHandler(float expirationTime) {
	this->expirationTime = expirationTime;
	playerLastUpdate = std::multimap<float, std::list<PlayerInfo>::iterator>();
	playerAddress = std::map<Address, std::list<PlayerInfo>::iterator>();
	playerInfoList = std::list<PlayerInfo>();
}

PlayerHandler::~PlayerHandler() {

}

bool PlayerHandler::isNewConnection(const Address& addr) const {
	return (playerAddress.find(addr) == playerAddress.end());
}

void PlayerHandler::updateConnection(const Address& addr) {
	if (isNewConnection(addr)) {
		insert(addr);
	} else {
		auto it = playerAddress.find(addr)->second->itLastUpdate;
		auto item = std::pair<float, std::list<PlayerInfo>::iterator>(it->first, it->second);
		item.first = Time::getClock();
		playerLastUpdate.erase(it);
		auto m_it = playerLastUpdate.insert(item);
		item.second->itLastUpdate = m_it;
	}
}

void PlayerHandler::removeExpiredConnections() {
	int elems = playerLastUpdate.size();
	for (auto i = playerLastUpdate.begin(); i != playerLastUpdate.end() and i->first < Time::getClock()-expirationTime; i++) {
		std::cout << "Jugador desconectado: " << i->second->itPlayerAddress->first.getAddressAsString() << std::endl;
		playerLastUpdate.erase(i);
		playerAddress.erase(i->second->itPlayerAddress);
		playerInfoList.erase(i->second);
	}
	elems -= playerLastUpdate.size();
}

void PlayerHandler::insert(const Address& addr) {
	PlayerInfo p;
	p.player = Player();
	playerInfoList.push_back(p);
	auto l_it = --playerInfoList.end();
	auto m_it1 = playerLastUpdate.insert(std::pair<float, std::list<PlayerInfo>::iterator>(Time::getClock(), l_it));
	auto m_it2 = playerAddress.insert(std::pair<Address, std::list<PlayerInfo>::iterator>(addr, l_it));
	l_it->itLastUpdate = m_it1;
	l_it->itPlayerAddress = m_it2.first;
}

Player& PlayerHandler::getPlayer(const Address& addr) {
	return playerAddress.find(addr)->second->player;
}
Player& PlayerHandler::getPlayer(const Address& addr) const {
	return playerAddress.find(addr)->second->player;
}

void PlayerHandler::printConnections() const {
	if (playerLastUpdate.size() > 0) {
		std::cout << "List of conections (Address) (>" << expirationTime << "):" << std::endl;
	}
	for (auto it = playerAddress.begin(); it != playerAddress.end(); ++it) {
		std::cout << it->first.getAddressAsString() << "\t" << Time::getClock()-it->second->itLastUpdate->first << std::endl;
	}
}

PlayerHandler::iterator PlayerHandler::begin() {
	return iterator(playerInfoList.begin());
}

PlayerHandler::iterator PlayerHandler::end() {
	return iterator(playerInfoList.end());
}

unsigned int PlayerHandler::size() {
	return playerInfoList.size();
}