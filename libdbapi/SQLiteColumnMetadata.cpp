#include "SQLiteColumnMetadata.h"

#include "sqlite3.h"

#include <stdexcept>

using std::out_of_range;

namespace up {
namespace db {

SQLiteColumnMetadata::SQLiteColumnMetadata(std::shared_ptr<SQLiteStatementHandle> statement, column_index index, column_index size):
	m_statement(statement),
	m_index(index),
	size(size)
{
}

SQLiteColumnMetadata::column_index SQLiteColumnMetadata::index() const
{
	return this->m_index;
}

char const *SQLiteColumnMetadata::name() const
{
	return ::sqlite3_column_name(*this->m_statement, this->m_index);
}

char const *SQLiteColumnMetadata::table() const
{
	return ::sqlite3_column_table_name(*this->m_statement, this->m_index);
}

bool SQLiteColumnMetadata::operator ==(SQLiteColumnMetadata const &instance) const
{
	return this->m_statement.get() == instance.m_statement.get()
		&& this->m_index == instance.m_index
		&& this->size == instance.size;
}

bool SQLiteColumnMetadata::operator !=(SQLiteColumnMetadata const &instance) const
{
	return this->m_statement.get() != instance.m_statement.get()
		|| this->m_index != instance.m_index
		|| this->size != instance.size;
}

SQLiteColumnMetadata &SQLiteColumnMetadata::operator ++()
{
	if(this->m_index >= this->size)
	{
		throw out_of_range("There are no elements after the last one");
	}

	++this->m_index;

	return *this;
}

SQLiteColumnMetadata &SQLiteColumnMetadata::operator --()
{
	if(!this->m_index)
	{
		throw out_of_range("There are no elements before the first one");
	}

	--this->m_index;

	return *this;
}

} // namespace db
} // namespace up
