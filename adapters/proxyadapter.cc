#include "proxyadapter.h"

ProxyAdapter::ProxyAdapter() {
	// "prime" the cache by forcing it to instantiate itself before any requests come in
	Cacher::getInstance();
}

ProxyAdapter::~ProxyAdapter() {

}

const char *ProxyAdapter::getName() {
	return "ProxyAdapter";
}

// this handles incoming requests on a per-client basis
int ProxyAdapter::onConnect(Client *base_client) {
	// first things first, we mint an HttpClient from our Client for convenience
	HttpClient client(base_client);
	
	while (true) {
		try {
			// step 1. receive and parse their request
			HttpRequest req = client.getHttpRequest();
			
			// if it's not a GET we have to return "Not Implemented"
			if (req.GetMethod() != HttpRequest::GET) {
				// send them a 'bad request' response
				client.sendHttpResponse(HttpToolbox::makeResponse("501", "Not Implemented"));
				// and move on to the next request
				continue;
			}
			
			// if it doesn't include a host (e.g. it's a relative URI), it's a bad request
			// similarly, if it doesn't include a path, it's bad too
			if (req.GetHost().length() <= 0 || req.GetPath().length() <= 0) {
				// send them a 'bad request' response
				client.sendHttpResponse(HttpToolbox::makeResponse("400", "Bad Request (Missing Host/Port)"));
				// and move on to the next request
				continue;
			}
			
			printf("Request for http://%s:%u%s...\n",
				req.GetHost().c_str(),
				req.GetPort(),
				req.GetPath().c_str());

			// step 2. spawn a thread to handle the request
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
			client.sendHttpResponse(HttpToolbox::makeResponse("400", "Bad Request"));

			// report and continue to service requests
			fprintf(stderr, "ParseException in ProxyAdapter::connect(): %s\n", e.what());
			continue;
		}
	}

	// done!
	return 0;
}

void ProxyAdapter::handleRequest(HttpClient client, HttpRequest& req) {
	Cacher &c = Cacher::getInstance();
	FullHttpResponse fullResponse;
	
	// DEBUG
	std::cout << "Made it to handleRequest()!" << std::endl << std::flush;
	
	try {
		// if this request should not be from the cache, return the response verbatim
		if (req.FindHeader("Cache-Control") == "no-cache") {
			// create a new client for the remote server to whom we'll be sending our calling client's request
			Client *temp_client = new Client(req.GetHost(), req.GetPort());
			HttpClient proxy_client(temp_client);
			proxy_client.sendHttpRequest(req);
			// get the response, but no caching here
			fullResponse = proxy_client.getHttpResponse();
			delete temp_client;
		}
		else {
			// it's cacheable, so attempt to either get it from the cache or whatever
			try {
				// DEBUG
				std::cout << "About to query cache for " << req.GetHost() << req.GetPath() << std::endl << std::flush;
				
				// retrieve from the cache
				CacheEntry entry = c.getCacheEntry(req.GetHost(), req.GetPort(), req.GetPath());
				
				std::cout << "**% headers (" << entry.GetHeaders().length() << "): " << std::endl << "--[" << entry.GetHeaders() << "]--" << std::endl << std::flush;
				
				// format it into the response object
				fullResponse.response.ParseResponse(entry.GetHeaders().c_str(), entry.GetHeaders().length());
				fullResponse.body = entry.GetBody();
			}
			catch (CacherException &e) {
				std::cout << "Cache MISS for " << req.GetHost() << req.GetPath() << std::endl << std::flush;
				
				// it's not in the cache or it's expired
				// so retrieve it, cache it, and set CacheEntry to that!
				
				// create a new client for the remote server to whom we'll be sending our calling client's request
				Client *temp_client = new Client(req.GetHost(), req.GetPort());
				HttpClient proxy_client(temp_client);
				proxy_client.sendHttpRequest(req);
				// get the response, cache it, then send it back to the caller
				fullResponse = proxy_client.getHttpResponse();
				
				// ...and create/store our new cache entry
				CacheEntry newEntry(req.GetHost(), req.GetPort(), req.GetPath());
				newEntry.SetModified(fullResponse.response.FindHeader("Last-Modified"));
				newEntry.SetExpires(fullResponse.response.FindHeader("Expires"));
				newEntry.SetHeaders(HttpToolbox::requestToString(req))
				newEntry.SetBody(fullResponse.body);
				c.addCacheEntry(newEntry);
				
				// and finally delete the temp client
				delete temp_client;
			}
		}
	}
	catch (HttpClientException &e) {
		// tell the caller what happened
		std::cerr << "HttpClientException: " << e.what() << std::endl << std::flush;
		// send the client a response and bail
		client.sendHttpResponse(HttpToolbox::makeResponse("400", std::string("Bad Request, ") + e.what()));
		return;
	}
	// FIXME: we should probably attempt to catch other kinds of exceptions here, too
	
	// we're guaranteed that FullResponse has something valid now
	// FIXME: use some kind of sync primitive to ensure that pipelined
	// requests are handled in the order that they're received
	client.sendHttpResponse(fullResponse);
}