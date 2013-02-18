#include "proxyadapter.h"

ProxyAdapter::ProxyAdapter() {
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
	
	// thread serialization (so we return GETs in the order they were received)
	int requestID = 0;
	// int currentID = 0;

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
				
			if (requestID >= MAX_CONCURRENT_REQUESTS) {
				// we can't service more than 10 requests per client
				std::cout << "Dropping additional request, as limit of "
					<< MAX_CONCURRENT_REQUESTS
					<< " concurrent requests has already been reached" << std::endl << std::flush;
				
				// send them a 'client error' response
				client.sendHttpResponse(HttpToolbox::makeResponse("429", "Too Many Requests"));
				// and move on to the next request (hopefully we'll have cleared things up by then?)
				continue;
			}

			// step 2. spawn a thread to handle the request
			boost::thread thr(boost::bind(&ProxyAdapter::handleRequest, this, client, req, requestID++));
			
			// ...optionally block for a given amount of time if REQUEST_THREAD_TIMEOUT_TIME > 0...
			if (REQUEST_THREAD_TIMEOUT_SECONDS > 0) {
				// we'll just get stuck here until the timer either expires or we 
				if (!thr.timed_join(boost::posix_time::seconds(REQUEST_THREAD_TIMEOUT_SECONDS))) {
					thr.interrupt();
					// REQUEST_THREAD_TIMEOUT_SECONDS seconds elapsed without a response; bail
					std::cout << "Request to remote server timed out after " << REQUEST_THREAD_TIMEOUT_SECONDS << ", aborting request" << std::endl << std::flush;
					client.sendHttpResponse(HttpToolbox::makeResponse("504", "Gateway Timeout"));
					continue;
				}
			}
			
			// ...and sleep for a second, since they're hitting us faster than we can cache, apparently
			sleep(1);
			
			// optional step 3. if we got a Connection: close header, exit the process now (closing the connection, too)
			if (req.FindHeader("Connection") == "close") {
				fprintf(stderr, "Client sent Connection: close header, exiting\n");
				break;
			}
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

void ProxyAdapter::handleRequest(HttpClient client, HttpRequest& req, int requestID) {
	// a handle to the db, opened here
	Cacher c;
	// the response we're eventually going to send
	FullHttpResponse fullResponse;
	
	try {
		// if this request should not be from the cache, return the response verbatim
		if (req.FindHeader("Cache-Control") == "no-cache") {
			// create a new client for the remote server to whom we'll be sending our calling client's request
			try {
				boost::scoped_ptr<Client> temp_client(new Client(req.GetHost(), req.GetPort(), REQUEST_TIMEOUT_SECONDS));
				HttpClient proxy_client(temp_client.get());
				proxy_client.sendHttpRequest(req);
				// get the response, but no caching here
				fullResponse = proxy_client.getHttpResponse();
			}
			catch (ClientTimeoutException &e) {
				// 30 seconds elapsed without a response; bail
				client.sendHttpResponse(HttpToolbox::makeResponse("504", "Gateway Timeout"));
				return;
			}
		}
		else {
			// it's cacheable, so attempt to either get it from the cache or whatever
			try {
				// retrieve from the cache
				CacheEntry entry = c.getCacheEntry(req.GetHost(), req.GetPort(), req.GetPath());
				
				// check if it's already expired; if so, we have to re-get it
				if (entry.isExpired())
					throw CacherExpiredException("Requested resource has expired in the cache, re-fetching...");
				
				// handle conditional gets here
				// if they supplied If-Modified-Since, check the cached object and only return it if it's newer
				if (req.FindHeader("If-Modified-Since") != "") {
					// attempt to parse the If-Modified-Since by passing it to entry.isModifiedAfter()
					try {
						if (!entry.isModifiedAfter(req.FindHeader("If-Modified-Since"), CacheEntry::HTTP_TIMESTAMP)) {
							// send them a 304 not modified, since we have no newer document
							client.sendHttpResponse(HttpToolbox::makeResponse("304", "Not Modified"));
							return;
						}
					}
					catch (CacheEntryInvalidTimestamp &e) {
						// just continue; their request is invalid
					}
				}

				// format it into the response object
				fullResponse.response.ParseResponse(entry.GetHeaders().c_str(), entry.GetHeaders().length());
				fullResponse.body = entry.GetBody();
			}
			catch (CacherException &e) {
				// it's not in the cache or it's expired
				// so retrieve it, cache it, and set CacheEntry to that!
				std::cout << "Cache miss for " << req.GetHost() << ":" << std::to_string(req.GetPort()) << req.GetPath() << std::endl << std::flush;
				
				// create a new client for the remote server to whom we'll be sending our calling client's request
				try {
					boost::scoped_ptr<Client> temp_client(new Client(req.GetHost(), req.GetPort(), REQUEST_TIMEOUT_SECONDS));
					HttpClient proxy_client(temp_client.get());
					proxy_client.sendHttpRequest(req);
					// get the response, cache it, then send it back to the caller
					fullResponse = proxy_client.getHttpResponse();
				}
				catch (ClientTimeoutException &e) {
					// 30 seconds elapsed without a response; bail
					client.sendHttpResponse(HttpToolbox::makeResponse("504", "Gateway Timeout"));
					return;
				}
				
				// ...and create/store our new cache entry
				CacheEntry newEntry(req.GetHost(), req.GetPort(), req.GetPath());
				newEntry.SetModified(fullResponse.response.FindHeader("Last-Modified"), CacheEntry::HTTP_TIMESTAMP);
				newEntry.SetExpires(fullResponse.response.FindHeader("Expires"), CacheEntry::HTTP_TIMESTAMP);
				newEntry.SetHeaders(HttpToolbox::responseToString(fullResponse.response));
				newEntry.SetBody(fullResponse.body);
				c.addCacheEntry(newEntry);
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