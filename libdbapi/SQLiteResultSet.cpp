#include "SQLiteResultSet.h"
#include "SQLiteDriver.h"
#include <cstring>
#include "sqlite3.h"

namespace up {
namespace db {

SQLiteResultSet::SQLiteResultSet():
	m_ready(false)
{
}

SQLiteResultSet::SQLiteResultSet(std::shared_ptr<SQLiteStatementHandle> handle):
	m_handle(handle),
	m_ready(false),
	columns(::sqlite3_column_count(*m_handle))
{
}

bool SQLiteResultSet::next()
{
	auto result = ::sqlite3_step(*this->m_handle);

	this->m_handle->executed();

	switch(result)
	{
		case SQLITE_DONE:
			this->m_ready = true;
			return false;

		case SQLITE_ROW:
			this->m_ready = true;
			return true;

		case SQLITE_BUSY:
			this->m_ready = false;
			return false;

		default:
			this->m_ready = false;
			SQLiteDriver::checkError(result);
			return false;
	}
}

SQLiteResultSet::column_index SQLiteResultSet::getColumns() const
{
    return this->columns;
}

bool SQLiteResultSet::ready() const
{
	return this->m_ready;
}

SQLiteResultSet::result_metadata SQLiteResultSet::metadata() const
{
	return SQLiteColumnsMetadata(this->m_handle);
}

SQLiteResultSet::result_column SQLiteResultSet::begin() const
{
	return SQLiteResultColumn(*this->m_handle, 0, this->columns);
}

SQLiteResultSet::result_column SQLiteResultSet::end() const
{
	return SQLiteResultColumn(*this->m_handle, this->columns, this->columns);
}

SQLiteResultSet::result_column SQLiteResultSet::operator[](column_index index) const
{
	return SQLiteResultColumn(*this->m_handle, index, this->columns);
}

} // namespace db
} // namespace up

