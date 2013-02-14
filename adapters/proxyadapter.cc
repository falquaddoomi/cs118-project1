#include "proxyadapter.h"

ProxyAdapter::ProxyAdapter() {

}

ProxyAdapter::~ProxyAdapter() {

}

const char *ProxyAdapter::getName() {
	return "ProxyAdapter";
}

// this handles incoming requests on a per-client basis
int ProxyAdapter::onConnect(Client *client) {
	while (true) {
		try {
			// step 1. receive their entire HTTP request
			std::string request = client->recvupto("\r\n\r\n");

			printf("\nYour data: %s\n", request.c_str());

			// step 2. parse the request
			HttpRequest req;

			req.ParseRequest(request.c_str(), request.length()+1);
			client->sendall("\nYour request: " + req.GetHost() + "/" + req.GetPath() + "\n");
			
			printf("Request for http://%s:%u%s...\n",
				req.GetHost().c_str(),
				req.GetPort(),
				req.GetPath().c_str());

			// step 3. spawn a thread to handle the request
			boost::thread thr(boost::bind(&ProxyAdapter::handleRequest, this, client, req));
		}
		catch (ClientDisconnectException &e) {
			// report and exit, since the client is gone
			fprintf(stderr, "Client disconnected, exiting child process\n");
			exit(0);
		}
		catch (ClientException &e) {
			// report and continue to service requests
			fprintf(stderr, "ClientException in ProxyAdapter::connect(): %s\n", e.what());
			continue;
		}
		catch (ParseException &e) {
			// send them a 'bad request' response
			client->sendall(HttpAgent::makeResponse("400", "Bad Request"));

			// report and continue to service requests
			fprintf(stderr, "ParseException in ProxyAdapter::connect(): %s\n", e.what());
			continue;
		}
	}

	// done!
	return 0;
}

void ProxyAdapter::handleRequest(Client *client, HttpRequest &req) {
	// first, check the cache
	// FIXME: figure out cache expiration later
	
	// actually, let's just serve them verbatim for now
	// we have to make a request to their target server for them
	try {
		// create a client and forward the request verbatim
		Client proxy_client(req.GetHost(), req.GetPort());
		proxy_client.sendall(HttpAgent::requestToString(req));
		
		// receive the response, then send that back to the client
		client->sendall(proxy_client.recvupto("\r\n\r\n"));
	}
	catch (ClientException &e) {
		// wat do?
		perror(e.what());
	}
}