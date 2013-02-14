#ifndef INCLUDE_up_db_SqlStatement_h
#define INCLUDE_up_db_SqlStatement_h

#include <string>

namespace up {
namespace db {

class SqlStatement {
private:
	std::string sql;
public:
	SqlStatement(std::string const & sql);
	SqlStatement(char const * sql = 0);

	SqlStatement & reset(char const * sql = 0);

	std::string str() const;
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SqlStatement_h
