#ifndef INCLUDE_up_db_SQLiteColumnsMetadata_h
#define INCLUDE_up_db_SQLiteColumnsMetadata_h

#include "SQLiteStatementHandle.h"
#include "SQLiteColumnMetadata.h"

#include <memory>

namespace up {
namespace db {

class SQLiteColumnsMetadata
{
public:
	typedef SQLiteColumnMetadata const_iterator;

	typedef SQLiteColumnMetadata::column_index column_index;

private:
	std::shared_ptr<SQLiteStatementHandle> m_handle;

public:
	SQLiteColumnsMetadata(std::shared_ptr<SQLiteStatementHandle> handle);

	const column_index columns;

	const_iterator begin() const;
	const_iterator end() const;

	const_iterator operator[](column_index index) const;
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteColumnsMetadata_h
