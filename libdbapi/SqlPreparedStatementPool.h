#ifndef INCLUDE_up_db_SqlPreparedStatementPool_h
#define INCLUDE_up_db_SqlPreparedStatementPool_h

#include <map>

namespace up {
namespace db {

template <typename TKey, typename TConnection>
class SqlPreparedStatementPool
{
public:
	typedef typename TConnection::statement_type::prepared_statement prepared_statement;

private:
	TConnection &m_connection;
	std::map<TKey, prepared_statement *> m_pool;

public:
	SqlPreparedStatementPool(TConnection &connection);
	~SqlPreparedStatementPool();

	prepared_statement &prepare(TKey key, char const *sql);

	prepared_statement &operator [](TKey key) const;
};

} // namespace db
} // namespace up

#include "SqlPreparedStatementPool.hpp"

#endif // INCLUDE_up_db_SqlPreparedStatementPool_h
