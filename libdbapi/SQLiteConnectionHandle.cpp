#include "SQLiteConnectionHandle.h"

#include "SQLiteDriver.h"

#include <stdexcept>

#include "sqlite3.h"

using std::runtime_error;

namespace up {
namespace db {

SQLiteConnectionHandle::SQLiteConnectionHandle():
	m_handle(0)
{
}

SQLiteConnectionHandle::~SQLiteConnectionHandle()
{
	if(this->m_handle)
	{
		SQLiteDriver::logError(sqlite3_close(this->m_handle));
	}
}

SQLiteConnectionHandle::operator handle_type *()
{
	return &this->m_handle;
}

SQLiteConnectionHandle::operator handle_type() const
{
	return this->m_handle;
}

bool SQLiteConnectionHandle::operator !() const
{
	return !this->m_handle;
}

SQLiteConnectionHandle::operator bool() const
{
	return this->m_handle != 0;
}

} // namespace db
} // namespace up
