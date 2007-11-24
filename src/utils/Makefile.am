

INCLUDES = -I$(top_srcdir)/src/

TESTS = testmoniker testfiles

check_PROGRAMS = testmoniker testfiles

testmoniker_SOURCES = testmoniker.cpp
testmoniker_LDADD = libniepceutils.la

testfiles_SOURCES = testfiles.cpp
testfiles_LDADD = libniepceutils.la \
	@BOOST_FILESYSTEM_LIB@


noinst_LTLIBRARIES = libniepceutils.la

libniepceutils_la_SOURCES = exception.h exception.cpp \
	mtqueue.h \
	buffer.h logstreamutils.h \
	moniker.h moniker.cpp \
	debug.h debug.cpp boost.h \
	files.h files.cpp \
	thread.h thread.cpp worker.h
