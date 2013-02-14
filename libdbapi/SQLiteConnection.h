#ifndef INCLUDE_up_db_SQLiteConnection_h
#define INCLUDE_up_db_SQLiteConnection_h

#include "SQLiteConnectionHandle.h"
#include "SQLiteStatement.h"
#include "SQLitePreparedStatement.h"
#include "BlobStreambuf.h"

#include <memory>
#include <string>

struct sqlite3;

namespace up {
namespace db {

class SQLiteConnection
{
public:
	typedef SQLiteStatement statement_type;
	typedef SQLitePreparedStatement preparedstatement_type;

private:
	std::shared_ptr<SQLiteConnectionHandle> m_connection;

public:
	SQLiteConnection(char const *db, bool create = false, bool readonly = true);

	statement_type statement(char const *sql);

	preparedstatement_type preparedStatement(char const *sql);

	preparedstatement_type preparedStatement(std::string const & sql);

	long long lastInsertId();

	void interrupt();

	std::shared_ptr<SQLiteConnectionHandle> getHandle();
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteConnection_h
