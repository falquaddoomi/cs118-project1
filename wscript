# -*- Mode: python; py-indent-offset: 4; indent-tabs-mode: nil; coding: utf-8; -*-

def options (opt):
    opt.load ('compiler_c compiler_cxx')
    opt.load ('boost')

def configure (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')

# comment out the following two lines if compiling not on lnxsrv
    env.options.boost_includes = '/u/cs/grad/yingdi/boost/include'
    env.options.boost_libs     = '/u/cs/grad/yingdi/boost/lib'
#

    env.check_boost(lib='thread')
    env.env.append_value('CXXFLAGS', ['-O0', '-g3', '-Wall', '-Werror', '-std=gnu++11'])

def build (env):
    env.load ('compiler_c compiler_cxx')
    env.load ('boost')

    server = env.program (
        target = "http-proxy",
        features = ["cxx", "cxxprogram"],
       	use = 'BOOST BOOST_THREAD BOOST_FILESYSTEM BOOST_DATE_TIME sqlite3',
		lib = ['sqlite3'],
        source = [
            # Put additional files here
            # ...
            #
            "http-proxy.cc", # main() function is here
            "http-headers.cc",
            "http-request.cc",
            "http-response.cc",
            
            "framework/socketserver.cc",
            "framework/protocoladapter.cc",
            "framework/client.cc",
			"framework/cacher.cc",
            "framework/agents/httpagent.cc",
			
			# libdbapi includes
			"libdbapi/BlobStreambuf.cpp",
			"libdbapi/BlobStream.cpp",
			"libdbapi/SQLiteBinder.cpp",
			"libdbapi/SQLiteColumnMetadata.cpp",
			"libdbapi/SQLiteColumnsMetadata.cpp",
			"libdbapi/SQLiteConnection.cpp",
			"libdbapi/SQLiteConnectionHandle.cpp",
			"libdbapi/SQLiteDriver.cpp",
			"libdbapi/SQLitePreparedStatement.cpp",
			"libdbapi/SQLiteResultColumn.cpp",
			"libdbapi/SQLiteResultSet.cpp",
			"libdbapi/SQLiteStatement.cpp",
			"libdbapi/SQLiteStatementHandle.cpp",
			"libdbapi/SqlStatement.cpp",

            "adapters/proxyadapter.cc"
            ]
        )
