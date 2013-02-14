#include "httpagent.h"

HttpAgent::HttpAgent() {

}

HttpAgent::~HttpAgent() {

}

std::string HttpAgent::makeResponse(std::string code, std::string message) {
	HttpResponse response;
	response.SetStatusCode(code);
	response.SetStatusMsg(message);
	response.SetVersion("1.1");

	// allocate buffer, which we'll delete in a hot second
	char *buffer = new char[response.GetTotalLength()];
	response.FormatResponse(buffer);
	std::string retval(buffer);
	delete[] buffer;

	// return a std::string, which actually makes sense to return (rawr)
	return retval;
}
