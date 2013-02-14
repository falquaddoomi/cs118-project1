#include "SqlStatement.h"

namespace up {
namespace db {

SqlStatement::SqlStatement(std::string const & sql):
    sql(sql)
{
}

SqlStatement::SqlStatement(char const * sql):
    sql(sql ? sql : "")
{
}

SqlStatement & SqlStatement::reset(char const * sql)
{
	this->sql = sql ? sql : "";
	return *this;
}

std::string SqlStatement::str() const
{
	return sql;
}

} // namespace db
} // namespace up
