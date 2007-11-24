

INCLUDES = -I$(top_srcdir)/src

noinst_LTLIBRARIES = liblibraryclient.la

check_PROGRAMS = test_worker
TESTS = test_worker
TEST_LIBS =  \
	liblibraryclient.la \
        ../library/libniepcelibrary.la \
	../db/libniepcedb.la ../db/sqlite/libniepcedbsqlite.la \
        ../utils/libniepceutils.la \
	../framework/libniepceframework.la \
        @BOOST_UNIT_TEST_FRAMEWORK_LIB@ \
	@BOOST_THREAD_LIB@ @BOOST_FILESYSTEM_LIB@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ @OPENRAW_LIBS@

test_worker_SOURCES = test_worker.cpp
test_worker_LDADD = $(TEST_LIBS)

liblibraryclient_la_SOURCES = libraryclient.h  libraryclient.cpp \
	clientimpl.h clientimpl.cpp \
	locallibraryserver.h locallibraryserver.cpp
