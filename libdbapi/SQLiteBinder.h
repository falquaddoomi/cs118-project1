#ifndef INCLUDE_up_db_SQLiteBinder_h
#define INCLUDE_up_db_SQLiteBinder_h

#include "SQLiteStatementHandle.h"

#include <memory>
#include <string>

namespace up {
namespace db {

class SQLiteBinder
{
public:
	typedef int bind_index;
	typedef int size_type;

	typedef int data_int;
	typedef long long data_bigint;
	typedef double data_double;
	typedef void *data_blob;

private:
	std::shared_ptr<SQLiteStatementHandle> m_handle;
	bind_index m_index;

public:
	SQLiteBinder(std::shared_ptr<SQLiteStatementHandle> handle, bind_index index = 0);

	SQLiteBinder &bind(data_int data, bool isUnsigned = false);
	SQLiteBinder &bind(unsigned data);
	SQLiteBinder &bind(data_bigint data, bool isUnsigned = false);
	SQLiteBinder &bind(data_double data, bool isUnsigned = false);

	SQLiteBinder &bind(char const *data, bool copy = false);
	SQLiteBinder &bind(char const *data, size_type size, bool copy = false);
	SQLiteBinder &bind(std::string const & data, bool copy = false);
	SQLiteBinder &bind(std::string && data);

	SQLiteBinder &bindBlob(size_type size);
	SQLiteBinder &bindBlob(data_blob data, size_type size, bool copy = false);

	SQLiteBinder &bindNull();
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteBinder_h
