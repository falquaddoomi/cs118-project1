#include "SQLitePreparedStatement.h"

#include "SQLiteDriver.h"

#include <stdexcept>

#include "sqlite3.h"

using std::runtime_error;

namespace up {
namespace db {

SQLitePreparedStatement::SQLitePreparedStatement()
{
}

SQLitePreparedStatement::SQLitePreparedStatement(SQLiteConnectionHandle const &connection, std::string const & sql):
	m_handle(new SQLiteStatementHandle)
{
	init(connection,sql.c_str(),sql.size());
}

SQLitePreparedStatement::SQLitePreparedStatement(SQLiteConnectionHandle const &connection, char const *sql, int length):
	m_handle(new SQLiteStatementHandle)
{
	init(connection,sql,length);
}

void SQLitePreparedStatement::reset(SQLiteConnectionHandle const &connection, std::string const & sql)
{
	m_handle.reset();
	m_handle.reset(new SQLiteStatementHandle);
	init(connection,sql.c_str(),sql.size());
}

void SQLitePreparedStatement::init(SQLiteConnectionHandle const &connection, char const *sql, int length)
{
	auto result = ::sqlite3_prepare_v2(connection, sql, length, *m_handle, 0);

	switch(result)
	{
		case SQLITE_OK:
			if(!this->m_handle)
			{
				throw runtime_error("Invalid handle returned while preparing the SQL statement");
			}
			break;

		default:
			SQLiteDriver::checkError(result);
	}
}

SQLitePreparedStatement::result_metadata SQLitePreparedStatement::metadata() const
{
	return SQLiteColumnsMetadata(this->m_handle);
}

SQLitePreparedStatement::parameter_binder SQLitePreparedStatement::binder(parameter_index index)
{
	return SQLiteBinder(this->m_handle, index);
}

SQLitePreparedStatement::result_set SQLitePreparedStatement::execute()
{
	this->m_handle->reset();

	return SQLiteResultSet(this->m_handle);
}

void SQLitePreparedStatement::executeUpdate()
{
	auto rs = execute();
	rs.next();			// next will actually call the command
}

} // namespace db
} // namespace up
