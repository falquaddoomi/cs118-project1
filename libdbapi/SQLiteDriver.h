#ifndef INCLUDE_up_db_SQLiteDriver_h
#define INCLUDE_up_db_SQLiteDriver_h

#include "SQLiteConnection.h"

#include <string>
#include <functional>

namespace up {
namespace db {

class SQLiteDriver
{
public:
	typedef SQLiteConnection connection_type;

private:
	std::string m_db;
	bool m_create;
	bool m_readonly;

public:
	SQLiteDriver(char const *db, bool create = false, bool readonly = true);

	connection_type connect() const;

	static connection_type dbConnect(char const *db, bool create = false, bool readonly = true);

	static void checkError(int code);

	static bool logError(int code);
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteDriver_h
