/*
==============================
= SocketServer
==============================
= Description:
= Creates a listening socket on construction, then
= handles accepting incoming connections and polling
= them for changes when loop() is called. Note that loop()
= will run endlessly, so it should be the last thing you call.
=
= When events occur on any of the connected sockets, SocketServer
= invokes the corresponding event method on each of the attached
= ProtocolAdapters in the order in which they were attached,
= stopping when one returns a >0 return code.
*/

#include "basic.h"

// project-specific includes
// the following is the adapter framework for registering adapters
#include "protocoladapter.h"
#include "client.h"

const int CONNECTION_BACKLOG = 10;

class SocketServer {
public:
	// initializes a new SocketServer on the given port
	SocketServer(int port);
	~SocketServer();

	// allows ProtocolAdapters to be used to interpret requests to this SocketServer
	void attach(ProtocolAdapter *adapter);

	// starts up the loop that accepts connections
	// and then endlessly monitors them for changes
	int loop();

protected:
	// we need to hold onto our listening port #
	int m_port;
	// ...and the listening socket that goes with it
	int m_listensock;

	// we also need to hold onto the list of adapters
	std::vector<ProtocolAdapter *> m_adapters;

	// starts listening on the port for connections
	// called by loop(), so there's no need to call this yourself
	int setup();
};
