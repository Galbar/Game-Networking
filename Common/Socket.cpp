// Copyright Alessio Linares
#include "./Socket.h"

int Socket::instanceCount = 0;

bool Socket::initializeSockets() {
	#if PLATFORM == PLATFORM_WINDOWS
	WSADATA WsaData;
	return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
	#else
	return true;
	#endif
}

void Socket::shutdownSockets() {
	#if PLATFORM == PLATFORM_WINDOWS
	WSACleanup();
	#endif
}

Socket::Socket() {
	if (instanceCount == 0)
		initializeSockets();
	++instanceCount;
}

Socket::~Socket() {
	this->close();
	--instanceCount;
	if (instanceCount == 0)
		shutdownSockets();
}

bool Socket::open(uint16_t port) {
	// Create socket and set it to UDP
	handle = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (handle <= 0) {
		return false;
	}
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons((uint16_t) port);

	if (bind(handle, (const sockaddr*) &address, sizeof(sockaddr_in)) < 0) {
		return false;
	}


	// Set socket in non-blocking mode
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

	int nonBlocking = 1;
	if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
		return false;
	}

	#elif PLATFORM == PLATFORM_WINDOWS

	DWORD nonBlocking = 1;
	if (ioctlsocket(handle, FIONBIO, &nonBlocking) != 0) {
		return false;
	}

	#endif
	return true;
}

void Socket::close() {
	#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
	::close(handle);
	#elif PLATFORM == PLATFORM_WINDOWS
	closesocket(handle);
	#endif
}

bool Socket::isOpen() const {
	return (handle >= 0);
}

bool Socket::send(const Address& destination, const void* data, int size) const{
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(destination.getAddress());
	address.sin_port = htons(destination.getPort());

	int bytes = sendto(handle, (const uint8_t*)(data), size, 0, reinterpret_cast<sockaddr*>(&address), sizeof(sockaddr_in));

	if (bytes != size) {
		return false;
	}
	return true;
}

int Socket::receive(Address* sender, void* data, int size) const{
	#if PLATFORM == PLATFORM_WINDOWS
	typedef int socklen_t;
	#endif

	sockaddr_in from;
	socklen_t fromLength = sizeof(from);

	int bytes = recvfrom(handle, reinterpret_cast<uint8_t*>(data), size, 0, reinterpret_cast<sockaddr*>(&from), &fromLength);

	uint32_t from_address = ntohl(from.sin_addr.s_addr);

	uint32_t from_port = ntohs(from.sin_port);

	*sender = Address(from_address, from_port);

	return bytes;
}
