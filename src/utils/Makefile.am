

INCLUDES = -I$(top_srcdir)/src/ @EXEMPI_CFLAGS@ @LIBXML2_CFLAGS@

TESTS = testmoniker testfiles testgeometry testxmp testufrawmeta

EXTRA_DIST = test.xmp test2.ufraw

check_PROGRAMS = testmoniker testfiles testgeometry testxmp testufrawmeta

testmoniker_SOURCES = testmoniker.cpp
testmoniker_LDADD = libniepceutils.la

testfiles_SOURCES = testfiles.cpp
testfiles_LDADD = libniepceutils.la \
	@BOOST_FILESYSTEM_LIBS@

testgeometry_SOURCES = testgeometry.cpp
testgeometry_LDADD = libniepceutils.la

testxmp_SOURCES = testxmp.cpp
testxmp_LDADD = libniepceutils.la \
	@EXEMPI_LIBS@ @BOOST_FILESYSTEM_LIBS@

testufrawmeta_SOURCES = testufrawmeta.cpp
testufrawmeta_CXXFLAGS =  -g -O0
testufrawmeta_LDADD = libniepceutils.la ../niepce/libniepceglobals.la \
	@EXEMPI_LIBS@ @BOOST_FILESYSTEM_LIBS@ @LIBXML2_LIBS@

noinst_LTLIBRARIES = libniepceutils.la

libniepceutils_la_SOURCES = exception.h exception.cpp \
	mtqueue.h exempi.h exempi.cpp \
	ufrawmeta.h ufrawmeta.cpp \
	buffer.h logstreamutils.h \
	moniker.h moniker.cpp \
	debug.h debug.cpp boost.h \
	files.h files.cpp \
	geometry.h geometry.cpp \
	thread.h thread.cpp worker.h \
	databinder.h databinder.cpp
