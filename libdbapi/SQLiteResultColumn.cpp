#include "SQLiteResultColumn.h"

#include <stdexcept>
#include <cstring>

#include "sqlite3.h"

using std::out_of_range;

namespace up {
namespace db {

SQLiteResultColumn::SQLiteResultColumn(SQLiteStatementHandle const &handle, column_index index, column_index size):
	m_handle(handle),
	m_index(index),
	size(size)
{
}

SQLiteResultColumn::column_index SQLiteResultColumn::index() const
{
	return this->m_index;
}

SQLiteResultColumn::data_type SQLiteResultColumn::type() const
	// TODO: Check for errors
{
	switch(::sqlite3_column_type(this->m_handle, this->m_index))
	{
		case SQLITE_INTEGER: return Integer;
		case SQLITE_FLOAT: return Real;
		case SQLITE_TEXT: return String;
		case SQLITE_BLOB: return Blob;
		case SQLITE_NULL: return Null;
		default: return Unknown;
	}
}

bool SQLiteResultColumn::isNull() const
{
	return ::sqlite3_column_type(this->m_handle, this->m_index) == SQLITE_NULL;
}

bool SQLiteResultColumn::notNull() const
{
	return ::sqlite3_column_type(this->m_handle, this->m_index) != SQLITE_NULL;
}

SQLiteResultColumn::data_int SQLiteResultColumn::getInt() const
	// TODO: Check for errors
	// TODO: Properly return unsigned results
{
	return ::sqlite3_column_int(this->m_handle, this->m_index);
}

SQLiteResultColumn::data_uint SQLiteResultColumn::getUnsignedInt() const
	// TODO: Check for errors
{
	return ::sqlite3_column_int(this->m_handle, this->m_index);
}

SQLiteResultColumn::data_bigint SQLiteResultColumn::getBigInt() const
	// TODO: Check for errors
	// TODO: Properly return unsigned results
{
	return ::sqlite3_column_int64(this->m_handle, this->m_index);
}

SQLiteResultColumn::data_ubigint SQLiteResultColumn::getUnsignedBigInt() const
	// TODO: Check for errors
{
	return ::sqlite3_column_int64(this->m_handle, this->m_index);
}

SQLiteResultColumn::data_double SQLiteResultColumn::getDouble() const
	// TODO: Check for errors
{
	return ::sqlite3_column_double(this->m_handle, this->m_index);
}

char const *SQLiteResultColumn::getCString() const
	// TODO: Check for errors
{
	return reinterpret_cast<char const *>(::sqlite3_column_text(this->m_handle, this->m_index));
}

char const *SQLiteResultColumn::getString(column_size &size) const
	// TODO: Check for errors
{
	size = ::sqlite3_column_bytes(this->m_handle, this->m_index);

	return reinterpret_cast<char const *>(::sqlite3_column_text(this->m_handle, this->m_index));
}

std::string SQLiteResultColumn::getString() const
	// TODO: Check for errors
{
	return std::string(reinterpret_cast<char const *>(::sqlite3_column_text(this->m_handle, this->m_index)), ::sqlite3_column_bytes(this->m_handle, this->m_index));
}

const SQLiteResultColumn::data_blob SQLiteResultColumn::getBlob(column_size &size) const
	// TODO: Check for errors
{
	size = ::sqlite3_column_bytes(this->m_handle, this->m_index);

	return const_cast<const data_blob>(::sqlite3_column_blob(this->m_handle, this->m_index));
}

bool SQLiteResultColumn::operator ==(SQLiteResultColumn const &instance) const
{
	return this->m_handle == instance.m_handle
		&& this->m_index == instance.m_index
		&& this->size == instance.size;
}

bool SQLiteResultColumn::operator !=(SQLiteResultColumn const &instance) const
{
	return this->m_handle != instance.m_handle
		|| this->m_index != instance.m_index
		|| this->size != instance.size;
}

SQLiteResultColumn &SQLiteResultColumn::operator ++()
{
	if(this->m_index >= this->size)
	{
		throw out_of_range("There are no parameters after the last one");
	}

	++this->m_index;

	return *this;
}

SQLiteResultColumn &SQLiteResultColumn::operator --()
{
	if(!this->m_index)
	{
		throw out_of_range("There are no parameters before the first one");
	}

	--this->m_index;

	return *this;
}

} // namespace db
} // namespace up
