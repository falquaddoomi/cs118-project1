#include "client.h"

Client::Client(int socket, sockaddr_storage addr, socklen_t addr_size) {
	m_socket = socket;
	m_addr = addr;
	m_addr_size = addr_size;
}

Client::~Client() {
	close(m_socket);
}

int Client::sendto(const void *msg, int len, int flags) {
	return send(m_socket, msg, len, flags);
}

int Client::recvfrom(void *buf, int len, int flags) {
	return recv(m_socket, buf, len, flags);
}

// continually send until we've sent the entire message
int Client::sendall(std::string msg) {
	unsigned int sofar = 0;

	while (sofar < msg.length()) {
		int retval = send(m_socket, msg.substr(sofar).c_str(), msg.substr(sofar).length(), 0);

		if (retval < 0)
			return retval;

		sofar += retval;
	}

	return sofar;
}