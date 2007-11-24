

SUBDIRS = sqlite

INCLUDES = -I$(top_srcdir)/src/ \
	@LIBGTKMM_CFLAGS@ @OPENRAW_CFLAGS@

TESTS = test_db test_library

TEST_LIBS =  \
	libniepcedb.la sqlite/libniepcedbsqlite.la \
	../utils/libniepceutils.la \
	../framework/libniepceframework.la \
	@BOOST_UNIT_TEST_FRAMEWORK_LIB@ \
	@BOOST_FILESYSTEM_LIB@ \
	@BOOST_THREAD_LIB@ \
	@LIBGTKMM_LIBS@ \
	@SQLITE3_LIBS@ @OPENRAW_LIBS@

check_PROGRAMS = test_db test_library

test_db_SOURCES = test_db.cpp
test_db_LDADD = $(TEST_LIBS)

test_library_SOURCE = test_library.cpp
test_library_LDADD = $(TEST_LIBS) 

noinst_LTLIBRARIES = libniepcedb.la

noinst_HEADERS = library.h iconnectiondriver.h iconnectionmanagerdriver.h \
	sqlstatement.h insertstatement.h

libniepcedb_la_SOURCES = library.cpp sqlstatement.cpp \
	insertstatement.cpp \
	libfile.h libfile.cpp \
	keyword.h keyword.cpp \
	thumbnailcache.h thumbnailcache.cpp \
	storage.h storage.cpp
