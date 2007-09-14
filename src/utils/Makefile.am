

INCLUDES = -I$(top_srcdir)/src/

TESTS = testmoniker

check_PROGRAMS = testmoniker
testmoniker_SOURCES = testmoniker.cpp
testmoniker_LDADD = libniepceutils.la

noinst_LTLIBRARIES = libniepceutils.la


libniepceutils_la_SOURCES = exception.h exception.cpp \
	buffer.h logstreamutils.h \
	moniker.h moniker.cpp \
	debug.h debug.cpp
