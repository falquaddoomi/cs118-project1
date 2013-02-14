#ifndef INCLUDE_up_db_SQLitePreparedStatement_h
#define INCLUDE_up_db_SQLitePreparedStatement_h

#include "SqlStatement.h"
#include "SQLiteConnectionHandle.h"
#include "SQLiteStatementHandle.h"
#include "SQLiteResultSet.h"
#include "SQLiteColumnsMetadata.h"
#include "SQLiteBinder.h"

#include <memory>
#include <string>

struct sqlite3;

namespace up {
namespace db {

class SQLitePreparedStatement: SqlStatement {
public:
	typedef SQLiteColumnsMetadata result_metadata;
	typedef SQLiteResultSet result_set;

	typedef SQLiteBinder parameter_binder;
	typedef parameter_binder::bind_index parameter_index;

private:
	std::shared_ptr<SQLiteStatementHandle> m_handle;
	void init(SQLiteConnectionHandle const &connection, char const *sql, int length);
public:
	SQLitePreparedStatement();
	SQLitePreparedStatement(SQLiteConnectionHandle const &connection, char const *sql, int length = -1);
	SQLitePreparedStatement(SQLiteConnectionHandle const &connection, std::string const & sql);

	void reset(SQLiteConnectionHandle const &connection, std::string const & sql);

	result_metadata metadata() const;

	parameter_binder binder(parameter_index index = 0);

	result_set execute();
	void executeUpdate();
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLitePreparedStatement_h
