#ifndef __PROXYADAPTER_H__
#define __PROXYADAPTER_H__

#include "../framework/basic.h"
#include "../framework/protocoladapter.h"
#include "../framework/httptoolbox.h"
#include "../framework/cache/cacher.h"

#include "../framework/clients/httpclient.h"

#include "../http-request.h"

// get boost stuff for threading and binding
#include <boost/bind.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/date_time.hpp>

const int MAX_CONCURRENT_REQUESTS = 10;
// amount of seconds to wait on a blocking connect() or recv()
// making this 0 causes sockets to not be set to time out at all
const int REQUEST_TIMEOUT_SECONDS = 0;
// amount of seconds to wait for a GET thread to complete
// making this non-zero serializes requests to the remote server
const int REQUEST_THREAD_TIMEOUT_SECONDS = 30;

class ProxyAdapter : public ProtocolAdapter {
public:
	ProxyAdapter();
	~ProxyAdapter();

	virtual const char* getName();

	virtual int onConnect(Client *);

protected:
	void handleRequest(HttpClient, HttpRequest&, int requestID);
};

#endif