

AM_CPPFLAGS = -I$(top_srcdir)/src/ \
	@LIBGLIBMM_CFLAGS@ @LIBGTKMM_CFLAGS@ \
	@OPENRAW_CFLAGS@ @EXEMPI_CFLAGS@

check_PROGRAMS = test_opqueue
TESTS = test_opqueue
TEST_LIBS =  \
        libniepcelibrary.a \
	../db/libniepcedb.a \
        $(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
        @BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@LIBGLIBMM_LIBS@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ @OPENRAW_LIBS@

noinst_LIBRARIES = libniepcelibrary.a

test_opqueue_SOURCES = test_opqueue.cpp opqueue.hpp
test_opqueue_LDADD = $(TEST_LIBS)


libniepcelibrary_a_SOURCES = clienttypes.hpp \
	op.hpp op.cpp  \
	commands.hpp commands.cpp \
	thumbnailcache.hpp thumbnailcache.cpp \
	thumbnailnotification.hpp
