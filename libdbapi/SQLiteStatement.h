#ifndef INCLUDE_up_db_SQLiteStatement_h
#define INCLUDE_up_db_SQLiteStatement_h

#include "SqlStatement.h"
#include "SQLitePreparedStatement.h"
#include "SQLiteResultSet.h"

#include <memory>

struct sqlite3;

namespace up {
namespace db {

class SQLiteStatement: public SqlStatement
{
public:
	typedef SQLitePreparedStatement prepared_statement;
	typedef SQLiteResultSet result_set;

private:
	std::shared_ptr<SQLiteConnectionHandle> m_connection;

public:
	SQLiteStatement(std::shared_ptr<SQLiteConnectionHandle> connection, char const *sql = 0);

	prepared_statement prepare();

	result_set execute();

	void executeUpdate();
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteStatement_h
