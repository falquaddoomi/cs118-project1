#include "protocoladapter.h"

ProtocolAdapter::ProtocolAdapter() {
	// set up the protocol adapter...generally nothing to do here
}

ProtocolAdapter::~ProtocolAdapter() {

}

const char *ProtocolAdapter::getName() {
	return "ProtocolAdapter";
}

int ProtocolAdapter::connect(Client *client) {
	return 0;
}