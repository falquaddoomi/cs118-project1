#ifndef __CACHER_H__
#define __CACHER_H__

#include "basic.h"

class CacheItem {
public:

protected:
	std::string modify_date;
};

class Cacher {
public:
	Cacher();
	~Cacher();

	const char CACHE_DIR[] = "cache";

	// check if an item is in the cache and return it if so



protected:
};

#endif