#ifndef INCLUDE_up_db_SQLiteColumnMetadata_h
#define INCLUDE_up_db_SQLiteColumnMetadata_h

#include "SQLiteStatementHandle.h"

#include <memory>

namespace up {
namespace db {

class SQLiteColumnMetadata
{
public:
	typedef int column_index;

private:
	std::shared_ptr<SQLiteStatementHandle> m_statement;
	column_index m_index;

public:
	SQLiteColumnMetadata(std::shared_ptr<SQLiteStatementHandle> statement, column_index index, column_index size);

	const column_index size;

	column_index index() const;

	char const *name() const;
	char const *table() const;

	bool operator ==(SQLiteColumnMetadata const &instance) const;
	bool operator !=(SQLiteColumnMetadata const &instance) const;

	SQLiteColumnMetadata &operator ++();
	SQLiteColumnMetadata &operator --();
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteColumnMetadata_h
