#include "SQLiteDriver.h"

#include <stdexcept>
#include <iostream>

#include "sqlite3.h"

using std::runtime_error;

namespace up {
namespace db {

SQLiteDriver::SQLiteDriver(char const *db, bool create, bool readonly):
	m_db(db),
	m_create(create),
	m_readonly(readonly)
{
}

SQLiteDriver::connection_type SQLiteDriver::connect() const
{
	return dbConnect(this->m_db.c_str(), this->m_create, this->m_readonly);
}

SQLiteDriver::connection_type SQLiteDriver::dbConnect(char const *db, bool create, bool readonly)
{
	return SQLiteConnection(db, create, readonly);
}

void SQLiteDriver::checkError(int code)
{
	switch(code)
	{
		case SQLITE_OK:
			return;

		case SQLITE_ERROR:
			throw runtime_error("SQL error or missing database");

		case SQLITE_INTERNAL:
			throw runtime_error("Internal logic error in SQLite");

		case SQLITE_PERM:
			throw runtime_error("Access permission denied");

		case SQLITE_ABORT:
			throw runtime_error("Callback routine requested an abort");

		case SQLITE_BUSY:
			throw runtime_error("The database file is locked");

		case SQLITE_LOCKED:
			throw runtime_error("A table in the database is locked");

		case SQLITE_NOMEM:
			throw runtime_error("A malloc() failed");

		case SQLITE_READONLY:
			throw runtime_error("Attempt to write a readonly database");

		case SQLITE_INTERRUPT:
			throw runtime_error("Operation terminated by sqlite3_interrupt(");

		case SQLITE_IOERR:
			throw runtime_error("Some kind of disk I/O error occurred");

		case SQLITE_CORRUPT:
			throw runtime_error("The database disk image is malformed");

		case SQLITE_NOTFOUND:
			throw runtime_error("NOT USED. Table or record not found");

		case SQLITE_FULL:
			throw runtime_error("Insertion failed because database is full");

		case SQLITE_CANTOPEN:
			throw runtime_error("Unable to open the database file");

		case SQLITE_PROTOCOL:
			throw runtime_error("Database lock protocol error");

		case SQLITE_EMPTY:
			throw runtime_error("Database is empty");

		case SQLITE_SCHEMA:
			throw runtime_error("The database schema changed");

		case SQLITE_TOOBIG:
			throw runtime_error("String or BLOB exceeds size limit");

		case SQLITE_CONSTRAINT:
			throw runtime_error("Abort due to constraint violation");

		case SQLITE_MISMATCH:
			throw runtime_error("Data type mismatch");

		case SQLITE_MISUSE:
			throw runtime_error("Library used incorrectly");

		case SQLITE_NOLFS:
			throw runtime_error("Uses OS features not supported on host");

		case SQLITE_AUTH:
			throw runtime_error("Authorization denied");

		case SQLITE_FORMAT:
			throw runtime_error("Auxiliary database format error");

		case SQLITE_RANGE:
			throw runtime_error("2nd parameter to sqlite3_bind out of range");

		case SQLITE_NOTADB:
			throw runtime_error("File opened that is not a database file");

		default:
			throw runtime_error("Unknown SQLite error");
	}
}

bool SQLiteDriver::logError(int code)
{
	try {
		checkError(code);
		return false;
	} catch(std::runtime_error & ex) {
		std::cerr << "DB ERROR: " << ex.what() << '\n';
	} catch(...) {
		std::cerr << "DB ERROR: unknown\n";
	}
	return true;
}


} // namespace db
} // namespace up
