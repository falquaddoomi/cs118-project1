#include "client.h"

#define max(a,b) ( ((a)>(b)) ? (a) : (b) )

Client::Client(int socket, sockaddr_storage addr, socklen_t addr_size) {
	m_socket = socket;
	m_addr = addr;
	m_addr_size = addr_size;
}

Client::Client(std::string host, unsigned short port) {
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
	
    // convert port number to a string (FIXME: this is stupid)
    std::stringstream ss;
    ss << port;
    std::string port_str = ss.str();

    if ((rv = getaddrinfo(host.c_str(), port_str.c_str(), &hints, &servinfo)) != 0) {
		throw ClientException(std::string("getaddrinfo: ") + gai_strerror(rv) + "\n");
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((m_socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(m_socket, p->ai_addr, p->ai_addrlen) == -1) {
            close(m_socket);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL)
        throw ClientException( "client: failed to connect\n");

    freeaddrinfo(servinfo); // all done with this structure
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

// continually send until we've sent the entire message, but binary style
void Client::sendall(std::vector<char> data) {
	unsigned int sofar = 0;
	
	while (sofar < data.size()) {
		int retval = send(m_socket, &data[sofar], data.size() - sofar, 0);

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
			throw ClientDisconnectException("Client::recvupto(): recv() returned <= 0, connection closed");

		// nul-terminate the buffer; we know this is always possible
		buffer[recvlen] = '\0';

		// copy the received data into the recv_buffer
		recv_buffer += buffer;

		// check if it contains the delimiter
		// FIXME: but we should search only from the last successful read for efficiency...
		size_t found_pos = recv_buffer.find(delimiter);

		if (found_pos != std::string::npos) {
			// put the remainder into the 'last buffer' holder
			// including the delimiter
			last_buffer = recv_buffer.substr(found_pos + delim_length);

			// remove the end from recv_buffer and return it
			recv_buffer.erase(found_pos + delim_length);
			
			/*
			// DEBUG
			std::replace(recv_buffer.begin(), recv_buffer.end(), '\r', 'r');
			std::replace(recv_buffer.begin(), recv_buffer.end(), '\n', 'n');

			
			// DEBUG
			std::cout << "**? last_buffer (" << last_buffer.length() << "): " << std::endl << "--[" << last_buffer << "]--" << std::endl << std::flush;
			std::cout << "**! recv_buffer (" << recv_buffer.length() << "): " << std::endl << "--[" << recv_buffer << "]--" << std::endl << std::flush;
			*/
						
			return recv_buffer;
		}

		// update the total bytes read cursor
		recvlen_total += recvlen;
	}

	// we return from within the while() (specifically when we've hit our delimiter), so no return here
}

std::vector<char> Client::recvupto(unsigned long bytes_to_read) {
	// create an accumulator that we'll return
	// this gets any leftover data from a previous call
	std::vector<char> result(last_buffer.cbegin(), last_buffer.cend());
	// also hold onto how many bytes we've read so far
	unsigned long sofar = result.size();
	// and finally init a place to store received data temporarily
	char buffer[2048];

	while (result.size() < bytes_to_read) {
		// read up to buffer - 1
		// (the "minus 1" is so we have a place to put the nul-terminator if we receive the max amount)
		int recvlen = recv(m_socket, buffer, sizeof buffer - 1, 0);

		if (recvlen <= 0)
			throw ClientException("Client::recvupto(): recv() returned <= 0, connection closed");

		// nul-terminate the buffer so that we can concatenate it to our accumulator
		buffer[recvlen] = '\0';

		// concat buffer
		result.reserve(result.size() + recvlen);
		result.insert(result.end(), buffer, buffer + recvlen);
		// increment count
		sofar += recvlen;
	}
	
	if (result.size() >= last_buffer.size())
		last_buffer.erase(); // we consumed the entire last_buffer
	else
		last_buffer.erase(result.size()); // we consumed only part of it
	
	// and return the total thing
	return result;
}