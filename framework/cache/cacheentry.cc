#include "cacheentry.h"

CacheEntry::CacheEntry() {
	
}

CacheEntry::CacheEntry(std::string host, unsigned short port, std::string path):
	m_host(host), m_port(port), m_path(path)
{
	
}

CacheEntry::~CacheEntry() {

}

// tests below

bool CacheEntry::isExpired() {
	time_t curtime;
	time(&curtime);
	return (m_expires < curtime);
}

bool CacheEntry::isModifiedAfter(std::string in_date, TimeConversion from_format) {
	struct tm tm;
	time_t comparetime;

	switch (from_format) {
		case CacheEntry::ISO8601:
			// parse the string as an ISO8601 timestamp
			if (strptime(in_date.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) == NULL)
				throw CacheEntryInvalidTimestamp(std::string("Invalid ISO8601 timestamp supplied: ") + in_date);
			comparetime = mktime(&tm);
			break;
		case CacheEntry::HTTP_TIMESTAMP:
		default:
			// parse the string as an HTTP GMT timestamp
			if (strptime(in_date.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm) == NULL)
				throw CacheEntryInvalidTimestamp(std::string("Invalid HTTP timestamp supplied: ") + in_date);
			comparetime = mktime(&tm);
			break;
	}
	
	return (m_modified > comparetime);
}

// setters below

void CacheEntry::SetHost(const std::string& host) {
	this->m_host = host;
}

void CacheEntry::SetPort(unsigned short port) {
	this->m_port = port;
}

void CacheEntry::SetPath(const std::string& path) {
	this->m_path = path;
}

void CacheEntry::SetModified(const std::string& modified, TimeConversion from_format) {
	struct tm tm;
	
	switch (from_format) {
		case CacheEntry::ISO8601:
			// parse the string as an ISO8601 timestamp
			if (strptime(modified.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) == NULL)
				throw CacheEntryInvalidTimestamp(std::string("Invalid ISO8601 timestamp supplied: ") + modified);
			this->m_modified = mktime(&tm);
			break;
		case CacheEntry::HTTP_TIMESTAMP:
		default:
			// parse the string as an HTTP GMT timestamp
			if (strptime(modified.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm) == NULL)
				throw CacheEntryInvalidTimestamp(std::string("Invalid HTTP timestamp supplied: ") + modified);
			this->m_modified = mktime(&tm);
			break;
	}
}

void CacheEntry::SetExpires(const std::string& expires, TimeConversion from_format) {
	struct tm tm;
	
	switch (from_format) {
		case CacheEntry::ISO8601:
			// parse the string as an ISO8601 timestamp
			if (strptime(expires.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) == NULL)
				throw CacheEntryInvalidTimestamp(std::string("Invalid ISO8601 timestamp supplied: ") + expires);
			this->m_expires = mktime(&tm);
			break;
		case CacheEntry::HTTP_TIMESTAMP:
		default:
			// parse the string as an HTTP GMT timestamp
			if (strptime(expires.c_str(), "%a, %d %b %Y %H:%M:%S GMT", &tm) == NULL)
				throw CacheEntryInvalidTimestamp(std::string("Invalid HTTP timestamp supplied: ") + expires);
			this->m_expires = mktime(&tm);
			break;
	}
}

void CacheEntry::SetHeaders(const std::string& headers) {
	this->m_headers = headers;
}

void CacheEntry::SetBody(const std::vector<char>& body) {
	this->m_body = body;
}

// getters below

const std::string& CacheEntry::GetHost() const {
	return m_host;
}

unsigned short CacheEntry::GetPort() const {
	return m_port;
}

const std::string& CacheEntry::GetPath() const {
	return m_path;
}

std::string CacheEntry::GetModified(TimeConversion to_format) const {
	char buffer[512];

	switch (to_format) {
		case CacheEntry::ISO8601:
			strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%S", gmtime(&m_modified));
			return std::string(buffer);
		case CacheEntry::HTTP_TIMESTAMP:
		default:
			strftime(buffer, sizeof buffer, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&m_modified));
			return std::string(buffer);
	}
}

std::string CacheEntry::GetExpires(TimeConversion to_format) const {
	char buffer[512];
	
	switch (to_format) {
		case CacheEntry::ISO8601:
			strftime(buffer, sizeof buffer, "%Y-%m-%dT%H:%M:%S", gmtime(&m_expires));
			return std::string(buffer);
		case CacheEntry::HTTP_TIMESTAMP:
		default:
			strftime(buffer, sizeof buffer, "%a, %d %b %Y %H:%M:%S GMT", gmtime(&m_expires));
			return std::string(buffer);
	}
}

const std::string& CacheEntry::GetHeaders() const {
	return m_headers;
}

const std::vector<char>& CacheEntry::GetBody() const {
	return m_body;
}
