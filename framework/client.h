#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "basic.h"

class Client {
public:
	Client(int socket, sockaddr_storage addr, socklen_t addr_size);
	~Client();

	// inlines
	int getSocket() { return m_socket; }

	// raw read/write methods
	int sendto(const void *msg, int len, int flags);
	int recvfrom(void *buf, int len, int flags);

	// helper methods
	int sendall(std::string msg);

protected:
	int m_socket;
    struct sockaddr_storage m_addr; // connector's address information
    socklen_t m_addr_size;
};

#endif