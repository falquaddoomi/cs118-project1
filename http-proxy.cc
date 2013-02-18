/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */

#include <iostream>

using namespace std;

#include "framework/socketserver.h"
#include "adapters/proxyadapter.h"

int main (int argc, char *argv[])
{
	// create instance of our helper socket server
	SocketServer server(14805);
	// create an instance of the adapter that'll handle connections
	// from the server and process them like an http proxy would
	ProxyAdapter adapter;

	// attach the proxy and loop forever
	// (which essentially means accepting connections and forking them to child processes,
	// which themselves are handled by the protocoladapter's connect() method)
	server.attach(&adapter);
	return server.loop();
}
