

SUBDIRS = sqlite

INCLUDES = -I$(top_srcdir)/src/

TESTS = test_db

check_PROGRAMS = test_db

test_db_SOURCES = test_db.cpp
test_db_LDADD = libniepcedb.la sqlite/libniepcedbsqlite.la \
	@BOOST_UNIT_TEST_FRAMEWORK_LIB@ \
	@SQLITE3_LIBS@

noinst_LTLIBRARIES = libniepcedb.la

noinst_HEADERS = library.h iconnectiondriver.h iconnectionmanagerdriver.h \
	sqlstatement.h exception.h insertstatement.h

libniepcedb_la_SOURCES = library.cpp sqlstatement.cpp exception.cpp \
	insertstatement.cpp