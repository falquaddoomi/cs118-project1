#include "socketserver.h"

// stuff we need to support beej's stuff
void sigchld_handler(int s)
{
    while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// ==================================
// === SocketServer implementation
// ==================================

SocketServer::SocketServer(int port) {
	// initialize the socket server here
	// or, rather, just set up our state
	m_port = port;
}

SocketServer::~SocketServer() {
	// we would deallocate the adapters,
	// except that we probably should leave that up to whoever allocated them
}

void SocketServer::attach(ProtocolAdapter *adapter) {
	// add the adapter to our vector of adapters to call
	// we'll manage the memory for the adapter, so there's no need to deallocate it
	m_adapters.push_back(adapter);
}

int SocketServer::setup() {
	// taken with some modification from beej's guide,
	// http://beej.us/guide/bgnet/output/html/multipage/clientserver.html#simpleserver

    struct addrinfo hints, *servinfo, *p;
    struct sigaction sa;
    int yes=1;
    int rv;

    // convert port number to a string (FIXME: this is stupid)
    std::stringstream ss;
    ss << m_port;
    std::string port_str = ss.str();

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, port_str.c_str(), &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((m_listensock = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(m_listensock, SOL_SOCKET, SO_REUSEADDR, &yes,
                sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(m_listensock, p->ai_addr, p->ai_addrlen) == -1) {
            close(m_listensock);
            perror("server: bind");
            continue;
        }

        break;
    }

    if (p == NULL)  {
        fprintf(stderr, "server: failed to bind\n");
        return 2;
    }

	// all done with this structure
    freeaddrinfo(servinfo);

    if (listen(m_listensock, CONNECTION_BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }

	// reap all dead processes
    sa.sa_handler = sigchld_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    return 0;
}

int SocketServer::loop() {
	int rv; // holds temp. return values so we can test them

	// set up our connection, first
	if ((rv = setup()) != 0) {
		fprintf(stderr, "error in SocketServer::setup(): %d\n", rv);
		return rv;
	}

	// loop endlessly and dispatch events to our protocol adapters
	printf("\n===========================================\n");
    printf("=== Waiting for connections on port %d...\n", m_port);
    printf("===========================================\n");

    printf("=== Registered Adapters: \n");
    for (std::vector<ProtocolAdapter *>::iterator it = m_adapters.begin(); it != m_adapters.end(); ++it) {
		printf("=== * %s\n", (*it)->getName());
	}
	printf("\n");

    // client's socket
    struct sockaddr_storage their_addr; // connector's address information
    char s[INET6_ADDRSTRLEN];
    socklen_t sin_size;
    int client_sock;

    while(1) {  // main accept() loop
        sin_size = sizeof their_addr;
        client_sock = accept(m_listensock, (struct sockaddr *)&their_addr, &sin_size);

        // accept() is a blocking call, meaning that it waits there until someone connects
        // since we're below accept(), either there was an error or we actually have a client now


        if (client_sock == -1) {
            perror("accept");
            continue;
        }

		// announce on the server
        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server: got connection from %s\n", s);

        if (fork() == 0) {
			// this is the child process
            close(m_listensock); // child doesn't need the listener

            // we assume that we have a client now; create a Client object to pass around
        	Client *client = new Client(client_sock, their_addr, sin_size);

            // loop through the attached protocol adapters,
            // passing this client to each one's connect() until one returns > 0
            for (std::vector<ProtocolAdapter *>::iterator it = m_adapters.begin(); it != m_adapters.end(); ++it) {
				if ((*it)->connect(client) > 0)
					break;
			}

            // and trash the client after
        	delete client;

            exit(0);
        }

        close(client_sock);  // parent doesn't need this
    }

    return 0;
}
