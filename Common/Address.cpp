// Copyright Alessio Linares
#include "./Address.h"

uint32_t Address::generateAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d) {
	return ( a << 24 ) | ( b << 16 ) | ( c << 8  ) | d;
}

Address::Address() {
	address = generateAddress(127, 0, 0, 1);
	port = 0;
}

Address::Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port) {
	address = generateAddress(a, b, c, d);
	this->port = port;
}

Address::Address(unsigned int address, uint16_t port) {
	this->address = address;
	this->port = port;
}

uint32_t Address::getAddress() const {
	return address;
}

std::string Address::getAddressAsString() const {
	std::stringstream ss;
	ss << static_cast<int>(getA()) << "." << static_cast<int>(getB()) << "." << static_cast<int>(getC()) << "." << static_cast<int>(getD()) << ":" << port;
	return ss.str();
}

uint8_t Address::getA() const {
	return ( address >> 24 ) & 0xFF;
}

uint8_t Address::getB() const {
	return ( address >> 16 ) & 0xFF;
}

uint8_t Address::getC() const {
	return ( address >> 8 ) & 0xFF;
}

uint8_t Address::getD() const {
	return address & 0xFF;
}

uint16_t Address::getPort() const {
	return port;
}

bool Address::operator==(const Address& r) const {
	return (address == r.address) and (port == r.port);
}

bool Address::operator!=(const Address& r) const {
	return (address != r.address) or (port != r.port);
}

bool Address::operator< (const Address& r) const {
	return (address < r.address) or (port < r.port);
}

bool Address::operator> (const Address& r) const {
	return (address > r.address) or (port > r.port);
}

bool Address::operator<=(const Address& r) const {
	return (*this < r) or (*this == r);
}

bool Address::operator>=(const Address& r) const {
	return (*this > r) or (*this == r);
}
