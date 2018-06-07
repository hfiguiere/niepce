
niepce_cppflags = -I$(top_srcdir)/src/ \
	@FRAMEWORK_CFLAGS@

AM_CPPFLAGS = $(niepce_cppflags)

TESTS = testfiles \
	testpathutils \
	teststringutils

EXTRA_DIST = test.xmp test2.ufraw MODULES_HOWTO

check_PROGRAMS = testfiles \
	testpathutils \
	teststringutils


testing_cppflags = -I@top_srcdir@/@GTEST_ROOT@/include $(niepce_cppflags)
testing_ldadd = @top_builddir@/third_party/libgtest.a \
	libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@

testfiles_SOURCES = t/testfiles.cpp
testfiles_CPPFLAGS = $(testing_cppflags)
testfiles_LDADD = $(testing_ldadd)

teststringutils_SOURCES = t/teststringutils.cpp
teststringutils_CPPFLAGS = $(testing_cppflags)
teststringutils_LDADD = $(testing_ldadd)

testpathutils_SOURCES = t/testpathutils.cpp
testpathutils_CPPFLAGS = $(testing_cppflags)
testpathutils_LDADD = $(testing_ldadd)

noinst_LIBRARIES = libniepceutils.a

libniepceutils_a_SOURCES = \
	init.hpp init.cpp \
	exception.hpp exception.cpp \
	fsutils.cpp fsutils.hpp \
	mtqueue.hpp stringutils.hpp \
	exempi.hpp exempi.cpp \
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

