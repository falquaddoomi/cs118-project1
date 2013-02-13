#ifndef __PROXYADAPTER_H__
#define __PROXYADAPTER_H__

#include "../framework/basic.h"
#include "../framework/protocoladapter.h"

#include "../http-request.h"

// the boost libs apparently have a lot of warnings, so we need to disable those first
#include <boost/bind.hpp>
#include <boost/thread.hpp>
// #include <boost/date_time.hpp>

class ProxyAdapter : public ProtocolAdapter {
public:
	ProxyAdapter();
	~ProxyAdapter();

	virtual const char* getName();

	virtual int connect(Client *);

protected:
	void handleRequest(Client *, HttpRequest &);
};

#endif