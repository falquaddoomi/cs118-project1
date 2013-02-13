#include "client.h"

#define max(a,b) ( ((a)>(b)) ? (a) : (b) )

Client::Client(int socket, sockaddr_storage addr, socklen_t addr_size) {
	m_socket = socket;
	m_addr = addr;
	m_addr_size = addr_size;
}

Client::~Client() {
	close(m_socket);
}

// continually send until we've sent the entire message
void Client::sendall(std::string msg) {
	unsigned int sofar = 0;

	while (sofar < msg.length()) {
		int retval = send(m_socket, msg.substr(sofar).c_str(), msg.substr(sofar).length(), 0);

		if (retval < 0)
			throw ClientException("Client::sendall(): send() returned non-zero value, e.g. error");

		sofar += retval;
	}
}

// continually receive until we encounter our delimiter
// it's an open question what we should do with the remainder of the buffer
std::string Client::recvupto(const char *delimiter) {
	// our current accumulator, starting with last time's buffer
	std::string recv_buffer = last_buffer;
	// the size of the delimiter we're reading
	size_t delim_length = strlen(delimiter);
	// the current number of bytes we've read
	size_t recvlen_total = 0;

	// loop until we break out on finding the delimiter
	while (true) {
		// read a lot of characters into our intermediate buffer
		char buffer[2048];
		int recvlen = recv(m_socket, &buffer, sizeof buffer - 1, 0);

		// if the remote disconnected, we'll get back 0
		// if an error occurred, we'll get back less than zero?
		if (recvlen <= 0)
			throw ClientDisconnectException("Client::recvupto(): recv() returned 0, connection closed");

		// nul-terminate the buffer; we know this is always possible
		buffer[recvlen] = '\0';

		// copy the received data into the recv_buffer
		recv_buffer += buffer;

		// check if it contains the delimiter
		// but search only from the last successful read
		size_t found_pos = recv_buffer.find(delimiter);

		if (found_pos != std::string::npos) {
			// put the remainder into the 'last buffer' holder
			// including the delimiter
			last_buffer = recv_buffer.substr(found_pos + delim_length);

			// remove the end from recv_buffer and return it
			recv_buffer.erase(found_pos + delim_length);
			return recv_buffer;
		}

		// update the total bytes read cursor
		recvlen_total += recvlen;
	}

	// there's no returning from here...
}
