#ifndef INCLUDE_up_db_SqlConnection_h
#define INCLUDE_up_db_SqlConnection_h

#include "SqlPreparedStatementPool.h"

#include <memory>

namespace up {
namespace db {

template <typename TKey, typename TConnection>
class SqlConnection
{
public:
	typedef TConnection connection_type;
	typedef SqlPreparedStatementPool<TKey, TConnection> pool_type;
	typedef typename connection_type::statement_type statement_type;
	typedef typename statement_type::prepared_statement prepared_statement;

private:
	connection_type m_connection;
	pool_type m_pool;

public:
	SqlConnection(TConnection const &connection);

	pool_type const &pool() const;
	pool_type &pool();

	connection_type const *operator ->() const;
	connection_type *operator ->();

	operator connection_type const &() const;
	operator connection_type &();
};

} // namespace db
} // namespace up

#include "SqlConnection.hpp"

#endif // INCLUDE_up_db_SqlConnection_h
