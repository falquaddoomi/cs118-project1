#include "httpagent.h"

HttpAgent::HttpAgent() {

}

HttpAgent::~HttpAgent() {

}

std::string HttpAgent::requestToString(HttpRequest request) {
	// allocate buffer, which we'll delete in a hot second
	char *buffer = new char[request.GetTotalLength()];
	request.FormatRequest(buffer);
	std::string retval(buffer);
	delete[] buffer;

	// return a std::string, which actually makes sense to return (rawr)
	return retval;
}

std::string HttpAgent::responseToString(HttpResponse response) {
	// allocate buffer, which we'll delete in a hot second
	char *buffer = new char[response.GetTotalLength()];
	response.FormatResponse(buffer);
	std::string retval(buffer);
	delete[] buffer;

	// return a std::string, which actually makes sense to return (rawr)
	return retval;
}

std::string HttpAgent::makeRequest(std::string host, std::string path, unsigned short port) {
	HttpRequest request;
	request.SetHost(host);
	request.SetMethod(HttpRequest::GET);
	request.SetPath(path);
	request.SetPort(port);

	return requestToString(request);
}

std::string HttpAgent::makeResponse(std::string code, std::string message) {
	HttpResponse response;
	response.SetStatusCode(code);
	response.SetStatusMsg(message);
	response.SetVersion("1.1");

	return responseToString(response);
}
