#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include <map>
#include <list>
#include "../Common/Address.h"
#include "../Common/Time.h"
#include "./Player.h"
class PlayerHandler
{
public:
	struct PlayerInfo
	{
		Player player;
		std::multimap<float, std::list<PlayerInfo>::iterator>::iterator itLastUpdate;
		std::map<Address, std::list<PlayerInfo>::iterator>::iterator itPlayerAddress;
	};

	struct iterator
	{
		Player* player;
		const Address* address;
		const float* time;

		iterator(std::list<PlayerInfo>::iterator it) {
			listIterator = it;
			update(listIterator);
		}

		void operator++() {
			listIterator++;
			update(listIterator);
		}

		void operator--() {
			listIterator--;
			update(listIterator);
		}

		bool operator!=(iterator it) {
			return (listIterator != it.listIterator);
		}

		bool operator==(iterator it) {
			return (listIterator == it.listIterator);
		}
	private:
		void update(std::list<PlayerInfo>::iterator it) {
			player = &(it->player);
			address = &(it->itPlayerAddress->first);
			time = &(it->itLastUpdate->first);
		}

		std::list<PlayerInfo>::iterator listIterator;
	};

	PlayerHandler(float expirationTime);
	~PlayerHandler();
	bool isNewConnection(const Address& addr) const;
	void updateConnection(const Address& addr);
	void removeExpiredConnections();
	Player& getPlayer(const Address& addr);
	Player& getPlayer(const Address& addr) const;
	void printConnections() const;

	iterator begin();
	iterator end();
	unsigned int size(); 

private:
	void insert(const Address& addr);
	float expirationTime;
	std::multimap<float, std::list<PlayerInfo>::iterator> playerLastUpdate;
	std::map<Address, std::list<PlayerInfo>::iterator> playerAddress;
	std::list<PlayerInfo> playerInfoList;
};
#endif
