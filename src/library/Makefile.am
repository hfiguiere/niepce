

INCLUDES = -I$(top_srcdir)/src/ \
	@LIBGTKMM_CFLAGS@ @OPENRAW_CFLAGS@ @EXEMPI_CFLAGS@

check_PROGRAMS = test_opqueue
TESTS = test_opqueue
TEST_LIBS =  \
        libniepcelibrary.la \
	../db/libniepcedb.la \
        ../utils/libniepceutils.la \
	../framework/libniepceframework.la \
        @BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@BOOST_THREAD_LIBS@ @BOOST_FILESYSTEM_LIBS@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ @OPENRAW_LIBS@

noinst_LTLIBRARIES = libniepcelibrary.la

test_opqueue_SOURCES = test_opqueue.cpp
test_opqueue_LDADD = $(TEST_LIBS)


libniepcelibrary_la_SOURCES = clienttypes.h \
	op.h op.cpp  \
	commands.h commands.cpp \
	thumbnailcache.h thumbnailcache.cpp \
	thumbnailnotification.h
