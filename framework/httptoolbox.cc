#include "httptoolbox.h"

HttpToolbox::HttpToolbox() {

}

HttpToolbox::~HttpToolbox() {

}

std::string HttpToolbox::requestToString(HttpRequest request) {
	// allocate buffer, which we'll delete in a hot second
	char *buffer = new char[request.GetTotalLength()];
	request.FormatRequest(buffer);
	std::string retval(buffer);
	delete[] buffer;

	// return a std::string, which actually makes sense to return (rawr)
	return retval;
}

std::string HttpToolbox::responseToString(HttpResponse response) {
	// allocate buffer, which we'll delete in a hot second
	char *buffer = new char[response.GetTotalLength()];
	response.FormatResponse(buffer);
	std::string retval(buffer);
	delete[] buffer;

	// return a std::string, which actually makes sense to return (rawr)
	return retval;
}

HttpRequest HttpToolbox::makeRequest(std::string host, std::string path, unsigned short port) {
	HttpRequest request;
	request.SetHost(host);
	request.SetMethod(HttpRequest::GET);
	request.SetPath(path);
	request.SetPort(port);

	return request;
}

HttpResponse HttpToolbox::makeResponse(std::string code, std::string message) {
	HttpResponse response;
	response.SetStatusCode(code);
	response.SetStatusMsg(message);
	response.SetVersion("1.1");

	return response;
}
