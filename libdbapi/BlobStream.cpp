#include "BlobStream.h"
#include "sqlite3.h"
#include <stdexcept>

namespace up {
namespace db {

template<typename S>
void do_open(S & s, SQLiteConnection & conn, char const * tableName, char const * columnName, long long row, bool readWrite)
{
	try {
		s.bsb.reset(new BlobStreambuf(*conn.getHandle(),"main",tableName,columnName,row,readWrite));
		s.rdbuf(s.bsb.get());
	} catch(std::runtime_error &) {
		s.close();
	}
}

template<typename S>
void do_close(S & s)
{
	s.rdbuf(0);
	s.bsb.reset();
	s.clear(std::ios_base::failbit);
}

Blob_istream::Blob_istream():
	std::istream(0)
{
}

Blob_istream::Blob_istream(Blob_istream && bis):
	std::istream(0),
	bsb(std::move(bis.bsb))
{
    rdbuf(bsb.get());
}

Blob_istream::Blob_istream(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row):
	std::istream(0)
{
	open(conn,tableName,columnName,row);
}

void Blob_istream::open(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row)
{
	do_open(*this,conn,tableName,columnName,row,false);
}

void Blob_istream::close()
{
	do_close(*this);
}


Blob_ostream::Blob_ostream():
	std::ostream(0)
{
}

Blob_ostream::Blob_ostream(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row):
	std::ostream(0)
{
	open(conn,tableName,columnName,row);
}

void Blob_ostream::open(SQLiteConnection & conn, char const * tableName, char const * columnName, long long row)
{
	do_open(*this,conn,tableName,columnName,row,true);
}

void Blob_ostream::close()
{
	do_close(*this);
}

} // namespace db
} // namespace up
