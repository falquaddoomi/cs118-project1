#ifndef INCLUDE_up_db_SQLiteResultColumn_h
#define INCLUDE_up_db_SQLiteResultColumn_h

#include "SQLiteStatementHandle.h"

#include <vector>
#include <string>

namespace up {
namespace db {

class SQLiteResultColumn
{
public:
	typedef int column_index;
	typedef int column_size;

	typedef enum {Unknown, Integer, Real, String, Blob, Null} data_type;

	typedef int data_int;
	typedef unsigned int data_uint;
	typedef long long data_bigint;
	typedef unsigned long long data_ubigint;
	typedef double data_double;
	typedef void *data_blob;

private:
	SQLiteStatementHandle const &m_handle;
	column_index m_index;

public:
	SQLiteResultColumn(SQLiteStatementHandle const &handle, column_index index, column_index size);

	const column_index size;

	column_index index() const;

	data_type type() const;

	bool isNull() const;
	bool notNull() const;

	data_int getInt() const;
	data_uint getUnsignedInt() const;
	data_bigint getBigInt() const;
	data_ubigint getUnsignedBigInt() const;
	data_double getDouble() const;

	char const *getCString() const;
	char const *getString(column_size &size) const;
	std::vector<char> &getString(std::vector<char> &s) const;
	std::string getString() const;

	const data_blob getBlob(column_size &size) const;

	bool operator ==(SQLiteResultColumn const &instance) const;
	bool operator !=(SQLiteResultColumn const &instance) const;

	SQLiteResultColumn &operator ++();
	SQLiteResultColumn &operator --();
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteResultColumn_h
