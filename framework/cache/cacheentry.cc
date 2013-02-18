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
	return false;
}

bool CacheEntry::isOlderThan(std::string in_date) {
	return false;
}

// setters below

void CacheEntry::SetExpires(const std::string& expires) {
	this->m_expires = expires;
}

void CacheEntry::SetHost(const std::string& host) {
	this->m_host = host;
}

void CacheEntry::SetModified(const std::string& modified) {
	this->m_modified = modified;
}

void CacheEntry::SetPath(const std::string& path) {
	this->m_path = path;
}

void CacheEntry::SetPort(unsigned short port) {
	this->m_port = port;
}

void CacheEntry::SetHeaders(const std::string& headers) {
	this->m_headers = headers;
}

void CacheEntry::SetBody(const std::vector<char>& body) {
	this->m_body = body;
}

// getters below

const std::string& CacheEntry::GetExpires() const {
	return m_expires;
}

const std::string& CacheEntry::GetHost() const {
	return m_host;
}

const std::string& CacheEntry::GetModified() const {
	return m_modified;
}

const std::string& CacheEntry::GetPath() const {
	return m_path;
}

unsigned short CacheEntry::GetPort() const {
	return m_port;
}

const std::string& CacheEntry::GetHeaders() const {
	return m_headers;
}

const std::vector<char>& CacheEntry::GetBody() const {
	return m_body;
}

std::vector<char> CacheEntry::GetMutableBody() const {
	return m_body;
}