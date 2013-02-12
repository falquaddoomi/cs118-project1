#ifndef __PROTOCOLADAPTER_H__
#define __PROTOCOLADAPTER_H__

#include "basic.h"

#include "client.h"

class ProtocolAdapter {
public:
	ProtocolAdapter();
	~ProtocolAdapter();

	virtual const char *getName();

	// interface functions for inheriting classes.
	// returning >0 from any of these causes the
	// chain of invocation to stop at this adapter
	virtual int connect(Client *client) = 0;

protected:
};

#endif