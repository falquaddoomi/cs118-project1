#ifndef __CACHEENTRY_H__
#define __CACHEENTRY_H__

#include "../basic.h"

class CacheEntry {
public:
	CacheEntry();
	CacheEntry(std::string host, unsigned short port, std::string path);
	~CacheEntry();

	bool isExpired();
	bool isOlderThan(std::string in_date);

	void SetExpires(const std::string& expires);
	void SetHost(const std::string& host);
	void SetModified(const std::string& modified);
	void SetPath(const std::string& path);
	void SetPort(unsigned short port);
	void SetHeaders(const std::string& headers);
	void SetBody(const std::vector<char>& body);

	const std::string& GetExpires() const;
	const std::string& GetHost() const;
	const std::string& GetModified() const;
	const std::string& GetPath() const;
	unsigned short GetPort() const;
	const std::string& GetHeaders() const;
	const std::vector<char>& GetBody() const;
	std::vector<char> GetMutableBody() const;
	
protected:
	std::string m_host;
	unsigned short m_port;
	std::string m_path;
	std::string m_modified;
	std::string m_expires;
	std::string m_headers;
	std::vector<char> m_body;
};

#endif
