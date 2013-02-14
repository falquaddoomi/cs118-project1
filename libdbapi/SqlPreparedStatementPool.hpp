#ifndef INCLUDE_up_db_SqlPreparedStatementPool_hpp
#define INCLUDE_up_db_SqlPreparedStatementPool_hpp

#include "SqlPreparedStatementPool.h"

#include <memory>

namespace up {
namespace db {

template <typename TKey, typename TConnection>
SqlPreparedStatementPool<TKey, TConnection>::SqlPreparedStatementPool(TConnection &connection):
	m_connection(connection)
{
}

template <typename TKey, typename TConnection>
SqlPreparedStatementPool<TKey, TConnection>::~SqlPreparedStatementPool()
{
	for(auto i = this->m_pool.begin(); i != this->m_pool.end(); ++i)
	{
		delete i->second;
	}

	this->m_pool.clear();
}

template <typename TKey, typename TConnection>
typename SqlPreparedStatementPool<TKey, TConnection>::prepared_statement &SqlPreparedStatementPool<TKey, TConnection>::prepare(TKey key, char const *sql)
{
	auto i = this->m_pool.find(key);

	if(i != this->m_pool.end())
	{
		delete i->second;
	}

	return *(this->m_pool[key] = this->m_connection.statement(sql).prepare());
}

template <typename TKey, typename TConnection>
typename SqlPreparedStatementPool<TKey, TConnection>::prepared_statement &SqlPreparedStatementPool<TKey, TConnection>::operator [](TKey key) const
{
	return *this->m_pool.find(key)->second;
}

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SqlPreparedStatementPool_hpp
