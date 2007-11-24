

INCLUDES = -I$(top_srcdir)/src/


check_PROGRAMS = test_opqueue
TESTS = test_opqueue
TEST_LIBS =  \
        libniepcelibrary.la \
	../db/libniepcedb.la ../db/sqlite/libniepcedbsqlite.la \
        ../utils/libniepceutils.la \
	../framework/libniepceframework.la \
        @BOOST_UNIT_TEST_FRAMEWORK_LIB@ \
	@BOOST_THREAD_LIB@ @BOOST_FILESYSTEM_LIB@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@

noinst_LTLIBRARIES = libniepcelibrary.la

test_opqueue_SOURCES = test_opqueue.cpp
test_opqueue_LDADD = $(TEST_LIBS)


libniepcelibrary_la_SOURCES = clienttypes.h \
	op.h op.cpp  \
	worker.h  \
	commands.h commands.cpp
