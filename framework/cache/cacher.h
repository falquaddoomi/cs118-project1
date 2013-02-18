#ifndef __CACHER_H__
#define __CACHER_H__

#include "../basic.h"
#include "../../libdbapi/SQLiteConnection.h"

#include "cacheentry.h"

class CacherException : public std::runtime_error {
public:
	CacherException(const std::string &msg):runtime_error(msg) { }
};

class CacherNotInCacheException : public CacherException {
public:
	CacherNotInCacheException(const std::string &msg):CacherException(msg) { }
};

class CacherExpiredException : public CacherException {
public:
	CacherExpiredException(const std::string &msg):CacherException(msg) { }
};

class Cacher {
public:
	Cacher();
	~Cacher();
	
	static const std::string DATABASE_NAME;

	// find an entry in the cache or NULL if it doesn't exist
	CacheEntry getCacheEntry(const std::string& host, unsigned short port, const std::string& path);
	
	// insert an entry into the cache, whether or not it exists
	void addCacheEntry(const CacheEntry& candidate);
	
protected:
	up::db::SQLiteConnection m_conn;
};

#endif