#ifndef INCLUDE_up_db_SQLiteStatementHandle_h
#define INCLUDE_up_db_SQLiteStatementHandle_h

struct sqlite3_stmt;

namespace up {
namespace db {

class SQLiteStatementHandle
{
public:
	typedef ::sqlite3_stmt *handle_type;

private:
	handle_type m_handle;
	bool m_executed;
	bool m_bound;

public:
	SQLiteStatementHandle();
	~SQLiteStatementHandle();

	void executed();
	void reset();

	void bound();
	void clearBindings();

	bool operator ==(SQLiteStatementHandle const &instance) const;
	bool operator !=(SQLiteStatementHandle const &instance) const;

	operator handle_type *();
	operator handle_type() const;

	bool operator !() const;
	operator bool() const;
};

} // namespace db
} // namespace up

#endif // INCLUDE_up_db_SQLiteStatementHandle_h
