#ifndef INCLUDE_up_db_SqlConnection_hpp
#define INCLUDE_up_db_SqlConnection_hpp

#include "SqlConnection.h"

namespace up {
namespace db {

template <typename TKey, typename TConnection>
SqlConnection<TKey, TConnection>::SqlConnection(TConnection const &connection):
	m_connection(connection),
	m_pool(m_connection)
{
}

template <typename TKey, typename TConnection>
typename SqlConnection<TKey, TConnection>::pool_type const &SqlConnection<TKey, TConnection>::pool() const
{
	return this->m_pool;
}

template <typename TKey, typename TConnection>
typename SqlConnection<TKey, TConnection>::pool_type &SqlConnection<TKey, TConnection>::pool()
{
	return this->m_pool;
}

template <typename TKey, typename TConnection>
typename SqlConnection<TKey, TConnection>::connection_type const *SqlConnection<TKey, TConnection>::operator ->() const
{
	return &this->m_connection;
}

template <typename TKey, typename TConnection>
typename SqlConnection<TKey, TConnection>::connection_type *SqlConnection<TKey, TConnection>::operator ->()
{
	return &this->m_connection;
}

template <typename TKey, typename TConnection>
SqlConnection<TKey, TConnection>::operator connection_type const &() const
{
	return this->m_connection;
}

template <typename TKey, typename TConnection>
SqlConnection<TKey, TConnection>::operator connection_type &()
{
	return this->m_connection;
}

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SqlConnection_hpp
