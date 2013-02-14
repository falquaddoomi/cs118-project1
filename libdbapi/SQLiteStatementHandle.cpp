#include "SQLiteStatementHandle.h"

#include "SQLiteDriver.h"

#include "sqlite3.h"

namespace up {
namespace db {

SQLiteStatementHandle::SQLiteStatementHandle():
	m_handle(0),
	m_executed(false),
	m_bound(false)
{
}

SQLiteStatementHandle::~SQLiteStatementHandle()
{
	if(this->m_handle)
	{
		SQLiteDriver::logError(::sqlite3_finalize(this->m_handle));
	}
}

void SQLiteStatementHandle::executed()
{
	this->m_executed = true;
}

void SQLiteStatementHandle::reset()
{
	if(this->m_executed)
	{
		// DIEGO removed error checking because sqlite3_reset returns the last error of the statement handle
		// that prevents reuse of statements when they fail due to reasons like constraint violations
//		SQLiteDriver::checkError(::sqlite3_reset(this->m_handle));
		::sqlite3_reset(this->m_handle);

		this->m_executed = false;
	}
}

void SQLiteStatementHandle::bound()
{
	this->m_bound = true;
}

void SQLiteStatementHandle::clearBindings()
{
	if(this->m_bound)
	{
		SQLiteDriver::checkError(::sqlite3_clear_bindings(this->m_handle));

		this->m_bound = false;
	}
}

bool SQLiteStatementHandle::operator ==(SQLiteStatementHandle const &instance) const
{
	return this->m_handle == instance.m_handle;
}

bool SQLiteStatementHandle::operator !=(SQLiteStatementHandle const &instance) const
{
	return this->m_handle != instance.m_handle;
}

SQLiteStatementHandle::operator handle_type *()
{
	return &this->m_handle;
}

SQLiteStatementHandle::operator handle_type() const
{
	return this->m_handle;
}

bool SQLiteStatementHandle::operator !() const
{
	return !this->m_handle;
}

SQLiteStatementHandle::operator bool() const
{
	return this->m_handle != 0;
}

} // namespace db
} // namespace up
