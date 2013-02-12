#ifndef __PROXYADAPTER_H__
#define __PROXYADAPTER_H__

#include "../framework/basic.h"
#include "../framework/protocoladapter.h"

class ProxyAdapter : public ProtocolAdapter {
public:
	ProxyAdapter();
	~ProxyAdapter();

	virtual const char* getName();

	virtual int connect(Client *);

protected:

};

#endif