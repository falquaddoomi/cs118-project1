#ifndef __HTTPREQUESTAGENT_H__
#define __HTTPREQUESTAGENT_H__

#include "../basic.h"
#include "../../http-request.h"
#include "../../http-response.h"

// intended to make requesting pages easier
class HttpAgent {
public:
	HttpAgent();
	~HttpAgent();

	static std::string makeRequest(std::string host, std::string path, unsigned short port=80);
	static std::string makeResponse(std::string code, std::string message);

	static std::string requestToString(HttpRequest request);
	static std::string responseToString(HttpResponse response);
	
protected:

};

#endif