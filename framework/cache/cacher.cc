#include "cacher.h"

const std::string Cacher::DATABASE_NAME = "cache.sqlite3";

Cacher::Cacher():m_conn(DATABASE_NAME.c_str(), true, false) {
	// we've already attempted to open sqlite database in the line above
	
	// now create our table if it doesn't already exist...
	m_conn.statement(
		"create table if not exists cache"
		"(hostname varchar(500) not null, port integer default 80, path varchar(500) not null, "
		"modified timestamp default current_timestamp, expires timestamp, "
		"headers text, body blob, "
		"primary key (hostname, port, path));"
	).executeUpdate();
}

Cacher::~Cacher() {
}

CacheEntry Cacher::getCacheEntry(const std::string& host, unsigned short port, const std::string& path) {
	// create our statement
	up::db::SQLitePreparedStatement getcache_stmt = m_conn.statement(
		"select hostname, port, path, modified, expires, headers, body from cache where hostname=? and port=? and path=?"
	).prepare();
	
	// attempt to look up the cache entry in the db
	getcache_stmt.binder()
		.bind(host.c_str())
		.bind(std::to_string(port))
		.bind(path.c_str());
	
	up::db::SQLitePreparedStatement::result_set r = getcache_stmt.execute();
	
	if (r.next()) {
		// process the result set into a new CacheEntry and return it
		CacheEntry temp(r[0].getString(), r[1].getInt(), r[2].getString());
		temp.SetModified(r[3].getString());
		temp.SetExpires(r[4].getString());
		temp.SetHeaders(r[5].getString());
		
		// copy the body blob into our body vector
		up::db::SQLiteResultColumn::column_size bodyblob_size;
		unsigned char* bodyblob = (unsigned char*)r[6].getBlob(bodyblob_size);
		temp.SetBody(std::vector<char>(bodyblob, bodyblob + bodyblob_size));

		return temp;
	}

	// since the above didn't return, we assume that there were no results
	throw CacherNotInCacheException("Couldn't find " + host + ":" + std::to_string(port) + "/" + path + " in cache");
}

void Cacher::addCacheEntry(const CacheEntry& candidate) {
	// create the statement that'll insert things
	up::db::SQLitePreparedStatement insertcache_stmt = m_conn.statement(
		"insert or replace into cache (hostname, port, path, modified, expires, headers, body) values (?, ?, ?, ?, ?, ?, ?)"
	).prepare();
	
	// copy body data into a temporary structure
	char blobBuffer[candidate.GetBody().size()+1];
	std::fill_n(blobBuffer, sizeof blobBuffer, 0);
	std::copy(candidate.GetBody().begin(), candidate.GetBody().end(), blobBuffer);

	// insert it into the db whether or not it exists
	insertcache_stmt.binder()
		.bind(candidate.GetHost())
		.bind(candidate.GetPort())
		.bind(candidate.GetPath())
		.bind(candidate.GetModified())
		.bind(candidate.GetExpires())
		.bind(candidate.GetHeaders())
		.bindBlob((void *)blobBuffer, candidate.GetBody().size());
	
	insertcache_stmt.executeUpdate();
}
