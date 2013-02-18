#include "httpclient.h"

/*
HttpClient::HttpClient(int socket, sockaddr_storage addr, socklen_t addr_size):Client(socket, addr, addr_size)  {
}

HttpClient::HttpClient(std::string host, unsigned short port):Client(host, port)  {
}
*/

HttpClient::HttpClient(Client* client) {
	m_client = client;
}

HttpClient::~HttpClient() {
}

HttpRequest HttpClient::getHttpRequest() {
	// read and attempt to parse the header
	std::string headers = m_client->recvupto("\r\n\r\n");

	HttpRequest req;
	req.ParseRequest(headers.c_str(), headers.length()); // throws a ParseException if something's wrong
	
	// and return it as-is, since there's no body (usually)
	return req;
}

FullHttpResponse HttpClient::getHttpResponse() {
	// allocate struct that we'll be returning
	FullHttpResponse fullResponse;
	
	// read and attempt to parse the header
	std::string headers = m_client->recvupto("\r\n\r\n");
	fullResponse.response.ParseResponse(headers.c_str(), headers.length()); // throws a ParseException if something's wrong
	
	try {
		// attempt to get the content length, if it exists
		long content_length = stol(fullResponse.response.FindHeader("Content-Length"));
		
		// read up to that number of bytes from the receiver and return that
		fullResponse.body = m_client->recvupto(content_length);
	}
	catch (std::invalid_argument &e) {
		throw HttpBadResponseException("Missing Content-Length header");
	}
	
	return fullResponse;
}

void HttpClient::sendHttpRequest(const HttpRequest& req) {
	m_client->sendall(HttpToolbox::requestToString(req));
}

void HttpClient::sendHttpResponse(const HttpResponse& resp) {
	m_client->sendall(HttpToolbox::responseToString(resp));
}

void HttpClient::sendHttpResponse(const FullHttpResponse& resp) {
	m_client->sendall(HttpToolbox::responseToString(resp.response));
	m_client->sendall(resp.body);
}
