#ifndef __HTTPTOOLBOX_H__
#define __HTTPTOOLBOX_H__

#include "basic.h"
#include "../http-request.h"
#include "../http-response.h"

// intended to make requesting pages easier
class HttpToolbox {
public:
	HttpToolbox();
	~HttpToolbox();

	static HttpRequest makeRequest(std::string host, std::string path, unsigned short port=80);
	static HttpResponse makeResponse(std::string code, std::string message);

	static std::string requestToString(HttpRequest request);
	static std::string responseToString(HttpResponse response);
	
protected:

};

#endif