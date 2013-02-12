#include "proxyadapter.h"

ProxyAdapter::ProxyAdapter() {

}

ProxyAdapter::~ProxyAdapter() {

}

const char *ProxyAdapter::getName() {
	return "ProxyAdapter";
}

int ProxyAdapter::connect(Client *client) {
	while (1) {
		client->sendall("Hi there; what's up?\n");
		sleep(1);
	}

	return 0;
}
