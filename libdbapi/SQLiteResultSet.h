#ifndef INCLUDE_up_db_SQLiteResultSet_h
#define INCLUDE_up_db_SQLiteResultSet_h

#include "SQLiteStatementHandle.h"
#include "SQLiteColumnsMetadata.h"
#include "SQLiteResultColumn.h"

#include <vector>
#include <string>
#include <memory>

namespace up {
namespace db {

class SQLiteResultSet
{
public:
	typedef SQLiteColumnsMetadata result_metadata;
	typedef SQLiteResultColumn result_column;

	typedef result_column::column_index column_index;

private:
	std::shared_ptr<SQLiteStatementHandle> m_handle;
	bool m_ready;
	column_index columns;

public:
    SQLiteResultSet();

	SQLiteResultSet(std::shared_ptr<SQLiteStatementHandle> statement);

    column_index getColumns() const;

	bool next();
	bool ready() const;

	result_metadata metadata() const;

	result_column begin() const;
	result_column end() const;

	result_column operator[](column_index index) const;
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteResultSet_h
