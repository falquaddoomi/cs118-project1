#include "SQLiteBinder.h"

#include "SQLiteDriver.h"

#include "sqlite3.h"

namespace up {
namespace db {

SQLiteBinder::SQLiteBinder(std::shared_ptr<SQLiteStatementHandle> handle, bind_index index):
	m_handle(handle),
	m_index(index)
{
	handle->reset();
}

SQLiteBinder &SQLiteBinder::bind(data_int data, bool isUnsigned)
{
	SQLiteDriver::checkError(::sqlite3_bind_int(*this->m_handle, ++this->m_index, data));

	this->m_handle->bound();

	return *this;
}

SQLiteBinder &SQLiteBinder::bind(unsigned data)
{
	return bind(data_int(data),true);
}

SQLiteBinder &SQLiteBinder::bind(data_bigint data, bool isUnsigned)
{
	SQLiteDriver::checkError(::sqlite3_bind_int64(*this->m_handle, ++this->m_index, data));

	this->m_handle->bound();

	return *this;
}

SQLiteBinder &SQLiteBinder::bind(data_double data, bool isUnsigned)
{
	SQLiteDriver::checkError(::sqlite3_bind_double(*this->m_handle, ++this->m_index, data));

	this->m_handle->bound();

	return *this;
}

SQLiteBinder &SQLiteBinder::bind(char const *data, bool copy)
{
	return bind(data, -1, copy);
}

SQLiteBinder &SQLiteBinder::bind(char const *data, size_type size, bool copy)
{
	SQLiteDriver::checkError(::sqlite3_bind_text(*this->m_handle, ++this->m_index, data, size, copy ? SQLITE_TRANSIENT : SQLITE_STATIC));

	this->m_handle->bound();

	return *this;
}

SQLiteBinder &SQLiteBinder::bind(std::string const & data, bool copy)
{
	return bind(data.c_str(),data.size(),copy);
}

SQLiteBinder &SQLiteBinder::bind(std::string && data)
{
	return bind(data.c_str(),data.size(),true);
}


SQLiteBinder &SQLiteBinder::bindBlob(size_type size)
{
	SQLiteDriver::checkError(::sqlite3_bind_zeroblob(*this->m_handle, ++this->m_index, size));

	this->m_handle->bound();

	return *this;
}

SQLiteBinder &SQLiteBinder::bindBlob(data_blob data, size_type size, bool copy)
{
	SQLiteDriver::checkError(::sqlite3_bind_blob(*this->m_handle, ++this->m_index, data, size, copy ? SQLITE_TRANSIENT : SQLITE_STATIC));

	this->m_handle->bound();

	return *this;
}

SQLiteBinder &SQLiteBinder::bindNull()
{
	SQLiteDriver::checkError(::sqlite3_bind_null(*this->m_handle, ++this->m_index));

	this->m_handle->bound();

	return *this;
}

} // namespace db
} // namespace up
