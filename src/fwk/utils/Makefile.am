
DIST_SUBDIRS = db

AM_CPPFLAGS = -I$(top_srcdir)/src/ \
	@FRAMEWORK_CFLAGS@

TESTS = testfiles testxmp \
	testpathutils \
	teststringutils test_db test_db2 test_db3 test_db4 testufrawmeta

EXTRA_DIST = test.xmp test2.ufraw MODULES_HOWTO

check_PROGRAMS = testfiles testxmp \
	testpathutils \
	teststringutils test_db test_db2 test_db3 test_db4 testufrawmeta


testfiles_SOURCES = testfiles.cpp
testfiles_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@


testxmp_SOURCES = testxmp.cpp
testxmp_LDADD = libniepceutils.a \
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


test_db_SOURCES = db/test_db.cpp
test_db_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@

test_db2_SOURCES = db/test_db2.cpp
test_db2_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@

test_db3_SOURCES = db/test_db3.cpp
test_db3_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
	@FRAMEWORK_LIBS@

test_db4_SOURCES = db/test_db4.cpp
test_db4_LDADD = libniepceutils.a \
	../base/libfwkbase.a \
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
	db/iconnectiondriver.hpp db/iconnectionmanagerdriver.hpp \
	db/insertstatement.cpp db/insertstatement.hpp \
	db/sqlstatement.cpp db/sqlstatement.hpp \
	db/sqlite/sqlitecnxdrv.cpp db/sqlite/sqlitecnxdrv.hpp \
	db/sqlite/sqlitecnxmgrdrv.cpp db/sqlite/sqlitecnxmgrdrv.hpp \
	dynamicmodule.hpp dynamicmodule.cpp \
	modulefactory.hpp \
	modulemanager.hpp modulemanager.cpp \
	$(NULL)

