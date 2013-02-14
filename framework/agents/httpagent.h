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

	static std::string makeResponse(std::string code, std::string message);

protected:

};

#endif