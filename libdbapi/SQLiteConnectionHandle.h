#ifndef INCLUDE_up_db_SQLiteConnectionHandle_h
#define INCLUDE_up_db_SQLiteConnectionHandle_h

struct sqlite3;

namespace up {
namespace db {

class SQLiteConnectionHandle
{
public:
	typedef ::sqlite3 *handle_type;

private:
	handle_type m_handle;

public:
	SQLiteConnectionHandle();
	~SQLiteConnectionHandle();

	operator handle_type *();
	operator handle_type() const;

	bool operator !() const;
	operator bool() const;
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteConnectionHandle_h
