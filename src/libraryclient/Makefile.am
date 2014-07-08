

AM_CPPFLAGS = -I$(top_srcdir)/src @FRAMEWORK_CFLAGS@

noinst_LIBRARIES = liblibraryclient.a

check_PROGRAMS = test_worker
TESTS = test_worker
TEST_LIBS =  \
	liblibraryclient.a \
        $(top_builddir)/src/engine/library/libniepcelibrary.a \
	$(top_builddir)/src/engine/db/libniepcedb.a \
        $(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
        @BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@FRAMEWORK_LIBS@ \
	@OPENRAW_LIBS@

test_worker_SOURCES = test_worker.cpp
test_worker_LDADD = $(TEST_LIBS)

liblibraryclient_a_SOURCES = libraryclient.hpp  libraryclient.cpp \
	clientimpl.hpp clientimpl.cpp \
	locallibraryserver.hpp locallibraryserver.cpp \
	uidataprovider.hpp uidataprovider.cpp \
	$(NULL)
