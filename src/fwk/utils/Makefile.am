
AM_CPPFLAGS = -I$(top_srcdir)/src/ \
	@FRAMEWORK_CFLAGS@

TESTS = testfiles \
	testpathutils \
	teststringutils testufrawmeta

EXTRA_DIST = test.xmp test2.ufraw MODULES_HOWTO

check_PROGRAMS = testfiles \
	testpathutils \
	teststringutils testufrawmeta


testfiles_SOURCES = testfiles.cpp
testfiles_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@


testufrawmeta_SOURCES = testufrawmeta.cpp
testufrawmeta_CXXFLAGS =  -g -O0
testufrawmeta_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@

teststringutils_SOURCES = teststringutils.cpp
teststringutils_LDADD =

testpathutils_SOURCES = t/testpathutils.cpp
testpathutils_LDADD = libniepceutils.a \
	@FRAMEWORK_LIBS@

noinst_LIBRARIES = libniepceutils.a

libniepceutils_a_SOURCES = \
	init.hpp init.cpp \
	exception.hpp exception.cpp \
	fsutils.cpp fsutils.hpp \
	mtqueue.hpp stringutils.hpp \
	exempi.hpp exempi.cpp \
	ufrawmeta.hpp ufrawmeta.cpp \
	buffer.hpp logstreamutils.hpp \
	boost.hpp \
	files.hpp files.cpp \
	thread.hpp thread.cpp worker.hpp \
	pathutils.hpp pathutils.cpp \
	databinder.hpp databinder.cpp \
	dynamicmodule.hpp dynamicmodule.cpp \
	modulefactory.hpp \
	modulemanager.hpp modulemanager.cpp \
	$(NULL)

