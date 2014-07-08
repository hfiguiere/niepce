

AM_CPPFLAGS = -I$(top_srcdir)/src/ \
	@FRAMEWORK_CFLAGS@ \
	@OPENRAW_CFLAGS@

check_PROGRAMS = test_opqueue
TESTS = test_opqueue
TEST_LIBS =  \
        libniepcelibrary.a \
	../db/libniepcedb.a \
        $(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
        @BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@FRAMEWORK_LIBS@ \
	@OPENRAW_LIBS@

noinst_LIBRARIES = libniepcelibrary.a

test_opqueue_SOURCES = test_opqueue.cpp opqueue.hpp
test_opqueue_LDADD = $(TEST_LIBS)


libniepcelibrary_a_SOURCES = clienttypes.hpp \
	op.hpp op.cpp  \
	commands.hpp commands.cpp \
	thumbnailcache.hpp thumbnailcache.cpp \
	thumbnailnotification.hpp
