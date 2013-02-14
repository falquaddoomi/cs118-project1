#include "SQLiteConnection.h"

#include "SQLiteDriver.h"

#include <cstring>

#include "sqlite3.h"

namespace up {
namespace db {

SQLiteConnection::SQLiteConnection(char const *db, bool create, bool readonly):
	m_connection(new SQLiteConnectionHandle)
{
	auto err = ::sqlite3_open_v2(db, *this->m_connection, (create ? SQLITE_OPEN_CREATE : 0) | (readonly ? SQLITE_OPEN_READONLY : SQLITE_OPEN_READWRITE), 0);
	SQLiteDriver::checkError(err);
}

SQLiteConnection::statement_type SQLiteConnection::statement(char const *sql)
{
	return SQLiteStatement(this->m_connection, sql);
}

SQLiteConnection::preparedstatement_type SQLiteConnection::preparedStatement(char const *sql)
{
	return SQLitePreparedStatement(*this->m_connection, sql);
}

SQLiteConnection::preparedstatement_type SQLiteConnection::preparedStatement(std::string const & sql)
{
	return SQLitePreparedStatement(*this->m_connection, sql);
}


long long SQLiteConnection::lastInsertId()
{
	return ::sqlite3_last_insert_rowid(*this->m_connection);
}

void SQLiteConnection::interrupt()
{
	::sqlite3_interrupt(*this->m_connection);
}

std::shared_ptr<SQLiteConnectionHandle> SQLiteConnection::getHandle()
{
	return m_connection;
}

} // namespace db
} // namespace up

