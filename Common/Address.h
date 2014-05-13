// Copyright Alessio Linares
#ifndef ADDRESS_HPP
#define ADDRESS_HPP
#include <cstdint>
#include <string>
#include <sstream>

class Address {
	public:
		Address();
		Address(uint8_t a, uint8_t b, uint8_t c, uint8_t d, uint16_t port);
		Address(uint32_t address, uint16_t port);
		uint32_t getAddress() const;
		std::string getAddressAsString() const;
		uint8_t getA() const;
		uint8_t getB() const;
		uint8_t getC() const;
		uint8_t getD() const;
		uint16_t getPort() const;
		bool operator==(const Address& r) const;
		bool operator!=(const Address& r) const;
		bool operator< (const Address& r) const;
		bool operator> (const Address& r) const;
		bool operator<=(const Address& r) const;
		bool operator>=(const Address& r) const;

	private:
		uint32_t generateAddress(uint8_t a, uint8_t b, uint8_t c, uint8_t d);
		uint32_t address;
		uint16_t port;
};
#endif
