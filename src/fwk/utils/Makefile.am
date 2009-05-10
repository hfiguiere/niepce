
DIST_SUBDIRS = db

INCLUDES = -I$(top_srcdir)/src/ @LIBGLIBMM_CFLAGS@ \
	@EXEMPI_CFLAGS@ @LIBXML2_CFLAGS@

TESTS = testfiles testxmp \
	testpathutils \
	teststringutils test_db test_db2 test_db3 test_db4 testufrawmeta

EXTRA_DIST = test.xmp test2.ufraw

check_PROGRAMS = testfiles testxmp \
	testpathutils \
	teststringutils test_db test_db2 test_db3 test_db4 testufrawmeta


testfiles_SOURCES = testfiles.cpp
testfiles_LDADD = libniepceutils.a \
	../libfwk.a \
	@LIBGLIBMM_LIBS@


testxmp_SOURCES = testxmp.cpp
testxmp_LDADD = libniepceutils.a \
	../libfwk.a \
	@EXEMPI_LIBS@ @LIBGLIBMM_LIBS@

testufrawmeta_SOURCES = testufrawmeta.cpp
testufrawmeta_CXXFLAGS =  -g -O0
testufrawmeta_LDADD = libniepceutils.a \
	../libfwk.a \
	@EXEMPI_LIBS@ @LIBXML2_LIBS@ \
	@LIBGLIBMM_LIBS@

teststringutils_SOURCES = teststringutils.cpp
teststringutils_LDADD = 

testpathutils_SOURCES = t/testpathutils.cpp
testpathutils_LDADD = libniepceutils.a \
	@LIBGLIBMM_LIBS@


test_db_SOURCES = db/test_db.cpp
test_db_LDADD = libniepceutils.a \
	../libfwk.a \
	@LIBGLIBMM_LIBS@\
	@SQLITE3_LIBS@

test_db2_SOURCES = db/test_db2.cpp
test_db2_LDADD = libniepceutils.a \
	../libfwk.a \
	@LIBGLIBMM_LIBS@\
	@SQLITE3_LIBS@

test_db3_SOURCES = db/test_db3.cpp
test_db3_LDADD = libniepceutils.a \
	../libfwk.a \
	@LIBGLIBMM_LIBS@\
	@SQLITE3_LIBS@

test_db4_SOURCES = db/test_db4.cpp
test_db4_LDADD = libniepceutils.a \
	../libfwk.a \
	@LIBGLIBMM_LIBS@\
	@SQLITE3_LIBS@

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
	db/sqlite/sqlitecnxmgrdrv.cpp db/sqlite/sqlitecnxmgrdrv.hpp

