#ifndef __CLIENT_H__
#define __CLIENT_H__

#include "../basic.h"
#include <stdexcept>

class ClientException : public std::runtime_error {
public:
	ClientException(const std::string &msg):runtime_error(msg) { }
};

class ClientTimeoutException : public ClientException {
public:
	ClientTimeoutException(const std::string &msg):ClientException(msg) { }
};

class ClientDisconnectException : public ClientException {
public:
	ClientDisconnectException(const std::string &msg):ClientException(msg) { }
};

class Client {
public:
	Client(int socket, sockaddr_storage addr, socklen_t addr_size);
	Client(std::string host, unsigned short port, long timeout_secs = 0);
	~Client();

	// inlines
	int getSocket() { return m_socket; }

	// helper methods
	void sendall(std::string msg);
	void sendall(std::vector<char> data);
	std::string recvupto(const char *delimiter);
	std::vector<char> recvupto(unsigned long bytes_to_read);

protected:
	int m_socket;
	bool m_istimeoutable; // true if the socket has a timeout value set
    struct sockaddr_storage m_addr; // connector's address information
    socklen_t m_addr_size;

    // data left over from previous reads
	std::string last_buffer;
};

#endif