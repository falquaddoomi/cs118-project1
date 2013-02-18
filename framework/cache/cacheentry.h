#ifndef __CACHEENTRY_H__
#define __CACHEENTRY_H__

#include "../basic.h"

#include <time.h>

class CacheEntryException : public std::runtime_error {
public:
	CacheEntryException(const std::string &msg):runtime_error(msg) { }
};

class CacheEntryInvalidTimestamp : public CacheEntryException {
public:
	CacheEntryInvalidTimestamp(const std::string &msg):CacheEntryException(msg) { }
};

class CacheEntry {
public:
	CacheEntry();
	CacheEntry(std::string host, unsigned short port, std::string path);
	~CacheEntry();
	
	// specifies what time format GetModified and GetExpires should return
	enum TimeConversion {
		ISO8601,
		HTTP_TIMESTAMP
	};

	bool isExpired();
	bool isModifiedAfter(std::string in_date, TimeConversion from_format);
	
	void SetHost(const std::string& host);
	void SetPort(unsigned short port);
	void SetPath(const std::string& path);
	void SetModified(const std::string& modified, TimeConversion from_format);
	void SetExpires(const std::string& expires, TimeConversion from_format);
	void SetHeaders(const std::string& headers);
	void SetBody(const std::vector<char>& body);
	
	const std::string& GetHost() const;
	unsigned short GetPort() const;
	const std::string& GetPath() const;
	std::string GetModified(TimeConversion to_format) const;
	std::string GetExpires(TimeConversion to_format) const;
	const std::string& GetHeaders() const;
	const std::vector<char>& GetBody() const;
	
protected:
	std::string m_host;
	unsigned short m_port;
	std::string m_path;
	time_t m_modified;
	time_t m_expires;
	std::string m_headers;
	std::vector<char> m_body;
};

#endif
