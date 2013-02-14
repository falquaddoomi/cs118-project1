#ifndef INCLUDE_up_db_BlobStreambuf_h
#define INCLUDE_up_db_BlobStreambuf_h

struct sqlite3_blob;

#include "SQLiteConnectionHandle.h"
#include <streambuf>
#include <memory>
#include <vector>

// TODO ver se eh viavel de implementar seekg/p e tellg/p
// TODO readsome?
// TODO setbuf?
// TODO change readWrite argument to an enum for supporting in, out and inout too. have to deal with pointers more judiciously
// TODO detach buffering code and use a decorator

namespace up {
namespace db {

class BlobStreambuf: public std::streambuf {
private:
	struct BlobDeleter {
		void operator()(sqlite3_blob * h) const;
	};
	std::unique_ptr<sqlite3_blob,BlobDeleter> handle;
	static const size_t DEFAULTBUFSIZE = 100;
	std::vector<char> buffer;
	size_t r,w,total;
public:
	BlobStreambuf(SQLiteConnectionHandle::handle_type connectionHandle, char const * db, char const * tableName,
		char const * columnName, long long row, bool readWrite);
	~BlobStreambuf();
	BlobStreambuf(BlobStreambuf &&) = default;
protected:
	virtual traits_type::int_type underflow();
	virtual traits_type::int_type overflow(int_type c = traits_type::eof());
	virtual std::streamsize showmanyc();
	virtual int sync();
	virtual std::streamsize xsgetn(char * s, std::streamsize n);
	virtual std::streamsize xsputn(char const * s, std::streamsize n);
private:
	bool failOnError(int code);
	bool readFromBlob(char * dest, size_t toRead);
	bool writeToBlob(char const * s, size_t toWrite);
};

} // namespace db
} // namespace up

#endif
