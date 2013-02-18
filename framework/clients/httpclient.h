#ifndef __HTTPCLIENT_H__
#define __HTTPCLIENT_H__

#include "../basic.h"

#include "../httptoolbox.h"
#include "../../http-request.h"
#include "../../http-response.h"
#include "client.h"

class HttpClientException : public std::runtime_error {
public:
	HttpClientException(const std::string &msg):runtime_error(msg) { }
};

class HttpBadRequestException : public HttpClientException {
public:
	HttpBadRequestException(const std::string &msg):HttpClientException(msg) { }
};

class HttpBadResponseException : public HttpClientException {
public:
	HttpBadResponseException(const std::string &msg):HttpClientException(msg) { }
};

/***
 * Stores HTTP headers as well as a binary body
 * 
 * Alright, yes, this is a glorified struct.
 * */
class FullHttpResponse {
public:
	HttpResponse response;
	std::vector<char> body;
};

/***
 * Wraps a reference to a Client to add helper methods for speaking HTTP
 * */
class HttpClient
{
public:
	HttpClient(Client* client);
	
	/*
	// restore these when we figure out how to make an HttpClient a clint
	HttpClient(int socket, sockaddr_storage addr, socklen_t addr_size):Client(socket, addr, addr_size);
	HttpClient(std::string host, unsigned short port):Client(host, port);
	*/
	
	~HttpClient();
	
	// convenience methods for sending/receiving requests/responses
	HttpRequest getHttpRequest();
	FullHttpResponse getHttpResponse();
	
	void sendHttpRequest(const HttpRequest &req);
	void sendHttpResponse(const HttpResponse &resp);
	void sendHttpResponse(const FullHttpResponse &resp);
	
protected:
	Client* m_client;
};

#endif // HTTPCLIENT_H
