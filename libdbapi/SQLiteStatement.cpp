#include "SQLiteStatement.h"
#include "sqlite3.h"

namespace up {
namespace db {

SQLiteStatement::SQLiteStatement(std::shared_ptr<SQLiteConnectionHandle> connection, char const *sql):
	SqlStatement(sql),
	m_connection(connection)
{
}

SQLiteStatement::prepared_statement SQLiteStatement::prepare()
{
	auto statement = str();

	return SQLitePreparedStatement(*this->m_connection, statement.data(), statement.length());
}

SQLiteStatement::result_set SQLiteStatement::execute()
{
	auto statement = str();

	return SQLitePreparedStatement(*this->m_connection, statement.data(), statement.length()).execute();
}

void SQLiteStatement::executeUpdate()
{
	auto rs = execute();
	rs.next();			// next will actually call the command
}


} // namespace db
} // namespace up
