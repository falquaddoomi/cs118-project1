#include "SQLiteColumnsMetadata.h"

#include "sqlite3.h"

namespace up {
namespace db {

SQLiteColumnsMetadata::SQLiteColumnsMetadata(std::shared_ptr<SQLiteStatementHandle> handle):
	m_handle(handle),
	columns(::sqlite3_column_count(*m_handle))
{
}

SQLiteColumnsMetadata::const_iterator SQLiteColumnsMetadata::begin() const
{
	return SQLiteColumnMetadata(this->m_handle, 0, this->columns);
}

SQLiteColumnsMetadata::const_iterator SQLiteColumnsMetadata::end() const
{
	return SQLiteColumnMetadata(this->m_handle, this->columns, this->columns);
}

SQLiteColumnsMetadata::const_iterator SQLiteColumnsMetadata::operator[](column_index index) const
{
	return SQLiteColumnMetadata(this->m_handle, index, this->columns);
}

} // namespace db
} // namespace up
