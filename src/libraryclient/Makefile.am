

INCLUDES = -I$(top_srcdir)/src @EXEMPI_CFLAGS@ @LIBGLIBMM_CFLAGS@

noinst_LIBRARIES = liblibraryclient.a

check_PROGRAMS = test_worker
TESTS = test_worker
TEST_LIBS =  \
	liblibraryclient.a \
        $(top_builddir)/src/engine/library/libniepcelibrary.a \
	$(top_builddir)/src/engine/db/libniepcedb.a \
        $(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/libfwk.a \
        @BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@LIBGLIBMM_LIBS@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ @OPENRAW_LIBS@ \
	@EXEMPI_LIBS@

test_worker_SOURCES = test_worker.cpp
test_worker_LDADD = $(TEST_LIBS)

liblibraryclient_a_SOURCES = libraryclient.hpp  libraryclient.cpp \
	clientimpl.hpp clientimpl.cpp \
	locallibraryserver.hpp locallibraryserver.cpp
