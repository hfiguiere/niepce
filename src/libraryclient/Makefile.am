

INCLUDES = -I$(top_srcdir)/src @EXEMPI_CFLAGS@

noinst_LIBRARIES = liblibraryclient.a

check_PROGRAMS = test_worker
TESTS = test_worker
TEST_LIBS =  \
	liblibraryclient.a \
        ../library/libniepcelibrary.a \
	../db/libniepcedb.a \
        ../utils/libniepceutils.a \
	../framework/libniepceframework.a \
        @BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@BOOST_THREAD_LIBS@ @BOOST_FILESYSTEM_LIBS@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ @OPENRAW_LIBS@ \
	@EXEMPI_LIBS@ @GNOMEVFS_LIBS@

test_worker_SOURCES = test_worker.cpp
test_worker_LDADD = $(TEST_LIBS)

liblibraryclient_a_SOURCES = libraryclient.h  libraryclient.cpp \
	clientimpl.h clientimpl.cpp \
	locallibraryserver.h locallibraryserver.cpp
