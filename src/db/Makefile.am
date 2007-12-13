

SUBDIRS = sqlite

INCLUDES = -I$(top_srcdir)/src/  @EXEMPI_CFLAGS@

TESTS = test_db test_library

TEST_LIBS =  \
	libniepcedb.la sqlite/libniepcedbsqlite.la \
	../utils/libniepceutils.la \
	../framework/libniepceframework.la \
	@LIBGTKMM_LIBS@\
	@BOOST_UNIT_TEST_FRAMEWORK_LIB@ \
	@BOOST_FILESYSTEM_LIB@ \
	@BOOST_THREAD_LIB@ \
	@SQLITE3_LIBS@ @EXEMPI_LIBS@

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
	storage.h storage.cpp
