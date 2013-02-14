#include "BlobStreambuf.h"

#include "SQLiteDriver.h"
#include "sqlite3.h"
#include <algorithm>

namespace up {
namespace db {

::sqlite3_blob * createHandle(SQLiteConnectionHandle::handle_type connectionHandle, char const * db, char const * tableName,
		char const * columnName, long long row, bool readWrite)
{
	::sqlite3_blob * h = 0;
	SQLiteDriver::checkError(::sqlite3_blob_open(connectionHandle,db,tableName,columnName,row,readWrite,&h));
	return h;
}

void BlobStreambuf::BlobDeleter::operator()(::sqlite3_blob * h) const
{
	SQLiteDriver::logError(::sqlite3_blob_close(h));
}


BlobStreambuf::BlobStreambuf(SQLiteConnectionHandle::handle_type connectionHandle, char const * db, char const * tableName,
		char const * columnName, long long row, bool readWrite):
	handle(createHandle(connectionHandle,db,tableName,columnName,row,readWrite))
{
	buffer.resize(DEFAULTBUFSIZE);
	total = ::sqlite3_blob_bytes(handle.get());
	if( readWrite ) {
		r = total;
		w = 0;
		auto bytesLeft = std::min(total,buffer.size());
		setp(&buffer[0],&buffer[bytesLeft]);
	} else {
		r = 0;
		w = total;
	}
}

BlobStreambuf::~BlobStreambuf()
{
	if( handle ) sync();
}

bool BlobStreambuf::failOnError(int code)
{
	if( ! SQLiteDriver::logError(code) ) return false;

	handle.reset();		// close the blob and stream will fail
	return true;
}

bool BlobStreambuf::readFromBlob(char * dest, size_t toRead)
{
	if( failOnError(::sqlite3_blob_read(handle.get(),dest,toRead,r)) ) return false;
	r += toRead;
	return true;
}

bool BlobStreambuf::writeToBlob(char const * s, size_t toWrite)
{
	if( failOnError(::sqlite3_blob_write(handle.get(),s,toWrite,w)) ) return false;
	w += toWrite;
	return true;
}

BlobStreambuf::traits_type::int_type BlobStreambuf::underflow()
{
	size_t toRead = static_cast<size_t>(showmanyc());			// possible loss of data my ass
	if( toRead == 0 ) return traits_type::eof();

	if( ! readFromBlob(&buffer[0],toRead) ) return traits_type::eof();

	setg(&buffer[0],&buffer[0],&buffer[toRead]);
	return buffer[0];
}

std::streamsize BlobStreambuf::showmanyc()
{
	return std::min(total-r,buffer.size());
}

std::streamsize BlobStreambuf::xsgetn(char * s, std::streamsize n)
{
	auto sp = s;
	// consume from buffer first
	auto inBuf = egptr() - gptr();
	if( inBuf > 0 ) {
		auto toRead = std::min<decltype(n)>(n,inBuf);
		std::copy(gptr(),gptr() + toRead,sp);
		sp += toRead;
		n -= toRead;
		gbump(toRead);
	}
	// if there are remaining chars to be read, read them from the blob field
	if( n > 0 ) {
		auto toRead = std::min<decltype(total)>(n,total-r);
		if( ! readFromBlob(sp,toRead) ) return traits_type::eof();

		sp += toRead;
	}

	return sp - s;
}


std::streamsize BlobStreambuf::xsputn(char const * s, std::streamsize n)
{
	auto sp = s;
	auto bufAvail = epptr() - pptr();
	if( n <= bufAvail ) {
		sp += n;
		std::copy(s,sp,pptr());
		pbump(n);
	} else {
		if( sync() == 0 ) {
			auto toWrite = std::min<decltype(total)>(n,total-w);
			if( writeToBlob(s,toWrite) ) {
				sp += toWrite;
			}
		}
	}
	return sp - s;
}

int BlobStreambuf::sync()
{
	auto toWrite = pptr() - pbase();
	if( toWrite == 0 ) return 0;

	if( ! writeToBlob(&buffer[0],toWrite) ) return -1;

	auto bytesLeft = std::min(total-w,buffer.size());
	setp(&buffer[0],&buffer[bytesLeft]);

	return 0;
}

BlobStreambuf::traits_type::int_type BlobStreambuf::overflow(int_type c)
{
	if( sync() != 0 || c == traits_type::eof() || w == total ) return traits_type::eof();

	buffer[0] = c;
	pbump(1);				// not sure why I had to do a pbump here. I thought uflow() should do that
	return buffer[0];
}


} // namespace db
} // namespace up

