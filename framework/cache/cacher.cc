#include "cacher.h"

const std::string Cacher::DATABASE_NAME = "cache.sqlite3";

Cacher::Cacher() {
	// attempt to open sqlite database
	m_conn = new up::db::SQLiteConnection(DATABASE_NAME.c_str(), true, false);
	
	// now create our table if it doesn't already exist...
	m_conn->statement(
		"create table if not exists cache"
		"(hostname varchar(500) not null, port integer default 80, path varchar(500) not null, "
		"modified timestamp default current_timestamp, expires timestamp, "
		"headers text, body blob, "
		"primary key (hostname, port, path));"
	).executeUpdate();
	
	// ...and prepare the statement that we'll be using to fetch things from the cache
	m_getcache_stmt = m_conn->preparedStatement(
		"select hostname, port, path, modified, expires, headers, body from cache where hostname=? and port=? and path=?"
	);

	// ...and prepare the statement that we'll be using to insert things into the cache
	m_insertcache_stmt = m_conn->preparedStatement(
		"insert or replace into cache (hostname, port, path, modified, expires, headers, body) values (?, ?, ?, ?, ?, ?, ?)"
	);
}

Cacher::~Cacher() {
	delete m_conn;
}

CacheEntry Cacher::getCacheEntry(const std::string& host, unsigned short port, const std::string& path) {
	// attempt to look up the cache entry in the db
	m_getcache_stmt.binder()
		.bind(host.c_str())
		.bind(std::to_string(port))
		.bind(path.c_str());
	
	up::db::SQLitePreparedStatement::result_set r = m_getcache_stmt.execute();
	
	if (r.next()) {
		std::cout << "Cacher found " << r[2].getString() << " for " << host << path << "..." << std::endl << std::flush;
		
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
	// insert it into the db whether or not it exists
	m_insertcache_stmt.binder()
		.bind(candidate.GetHost())
		.bind(candidate.GetPort())
		.bind(candidate.GetPath())
		.bind(candidate.GetModified())
		.bind(candidate.GetExpires())
		.bind(candidate.GetHeaders())
		.bindBlob(&candidate.GetMutableBody()[0], candidate.GetBody().size());
	
	m_insertcache_stmt.executeUpdate();
}
