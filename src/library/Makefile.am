

INCLUDES = -I$(top_srcdir)/src/


check_PROGRAMS = test_opqueue test_worker
TESTS = test_opqueue test_worker
TEST_LIBS =  \
        libniepcelibrary.la \
	../db/libniepcedb.la ../db/sqlite/libniepcedbsqlite.la \
        ../utils/libniepceutils.la \
        @BOOST_UNIT_TEST_FRAMEWORK_LIB@ \
	@BOOST_THREAD_LIB@ \
	@SQLITE3_LIBS@

noinst_LTLIBRARIES = libniepcelibrary.la

test_opqueue_SOURCES = test_opqueue.cpp
test_opqueue_LDADD = $(TEST_LIBS)

test_worker_SOURCES = test_worker.cpp
test_worker_LDADD = $(TEST_LIBS)

libniepcelibrary_la_SOURCES = libfile.h libfile.cpp \
	keyword.h keyword.cpp \
	storage.h storage.cpp \
	op.h op.cpp opqueue.h opqueue.cpp \
	worker.h worker.cpp
