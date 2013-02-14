#ifndef __PROXYADAPTER_H__
#define __PROXYADAPTER_H__

#include "../framework/basic.h"
#include "../framework/protocoladapter.h"
#include "../framework/agents/httpagent.h"

#include "../http-request.h"

// get boost stuff for threading and binding
// the boost libs apparently have a lot of warnings, so we need to disable those first
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

class ProxyAdapter : public ProtocolAdapter {
public:
	ProxyAdapter();
	~ProxyAdapter();

	virtual const char* getName();

	virtual int onConnect(Client *);

protected:
	void handleRequest(Client *, HttpRequest &);
};

#endif