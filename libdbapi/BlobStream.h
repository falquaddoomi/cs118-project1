#ifndef INCLUDE_up_db_BLOB_STREAM_H_GUARD
#define INCLUDE_up_db_BLOB_STREAM_H_GUARD

#include <istream>
#include "SQLiteConnection.h"
#include "BlobStreambuf.h"

namespace up {
namespace db {

class Blob_istream: public std::istream {
	template<typename S>
	friend void do_open(S & s, SQLiteConnection & conn, char const * tableName, char const * columnName, long long row, bool readWrite);
	template<typename S>
	friend void do_close(S & s);
public:
	Blob_istream();
	Blob_istream(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row);
	Blob_istream(Blob_istream && bis);
	void open(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row);
	void close();
private:
	std::unique_ptr<BlobStreambuf> bsb;
};

class Blob_ostream: public std::ostream {
	template<typename S>
	friend void do_open(S & s, SQLiteConnection & conn, char const * tableName, char const * columnName, long long row, bool readWrite);
	template<typename S>
	friend void do_close(S & s);
public:
	Blob_ostream();
	Blob_ostream(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row);
	void open(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row);
	void close();
private:
	std::unique_ptr<BlobStreambuf> bsb;
};

} // namespace db
} // namespace up

#endif

