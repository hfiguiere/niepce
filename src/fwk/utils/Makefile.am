
DIST_SUBDIR = db

INCLUDES = -I$(top_srcdir)/src/ @EXEMPI_CFLAGS@ @LIBXML2_CFLAGS@

TESTS = testmoniker testfiles testgeometry testxmp \
	teststringutils test_db test_db2 test_db3 test_db4 testufrawmeta

EXTRA_DIST = test.xmp test2.ufraw

check_PROGRAMS = testmoniker testfiles testgeometry testxmp \
	teststringutils test_db test_db2 test_db3 test_db4 testufrawmeta

testmoniker_SOURCES = testmoniker.cpp
testmoniker_LDADD = libniepceutils.a \
	@BOOST_THREAD_LIBS@

testfiles_SOURCES = testfiles.cpp
testfiles_LDADD = libniepceutils.a \
	@BOOST_FILESYSTEM_LIBS@ @BOOST_THREAD_LIBS@

testgeometry_SOURCES = testgeometry.cpp
testgeometry_LDADD = libniepceutils.a \
	@BOOST_THREAD_LIBS@

testxmp_SOURCES = testxmp.cpp
testxmp_LDADD = libniepceutils.a \
	@EXEMPI_LIBS@ @BOOST_FILESYSTEM_LIBS@ @BOOST_THREAD_LIBS@

testufrawmeta_SOURCES = testufrawmeta.cpp
testufrawmeta_CXXFLAGS =  -g -O0
testufrawmeta_LDADD = libniepceutils.a \
	@EXEMPI_LIBS@ @BOOST_FILESYSTEM_LIBS@ @LIBXML2_LIBS@ \
	@BOOST_THREAD_LIBS@

teststringutils_SOURCES = teststringutils.cpp
teststringutils_LDADD = 

test_db_SOURCES = db/test_db.cpp
test_db_LDADD = libniepceutils.a \
	@BOOST_THREAD_LIBS@\
	@SQLITE3_LIBS@

test_db2_SOURCES = db/test_db2.cpp
test_db2_LDADD = libniepceutils.a \
	@BOOST_THREAD_LIBS@\
	@SQLITE3_LIBS@

test_db3_SOURCES = db/test_db3.cpp
test_db3_LDADD = libniepceutils.a \
	@BOOST_THREAD_LIBS@\
	@SQLITE3_LIBS@

test_db4_SOURCES = db/test_db4.cpp
test_db4_LDADD = libniepceutils.a \
	@BOOST_THREAD_LIBS@\
	@SQLITE3_LIBS@

noinst_LIBRARIES = libniepceutils.a

libniepceutils_a_SOURCES = exception.h exception.cpp \
	mtqueue.h stringutils.h \
	autoflag.h \
	exempi.h exempi.cpp \
	ufrawmeta.h ufrawmeta.cpp \
	buffer.h logstreamutils.h \
	moniker.h moniker.cpp \
	debug.h debug.cpp boost.h \
	files.h files.cpp \
	geometry.h geometry.cpp \
	thread.h thread.cpp worker.h \
	databinder.h databinder.cpp \
	db/iconnectiondriver.h db/iconnectionmanagerdriver.h \
	db/insertstatement.cpp db/insertstatement.h\
	db/sqlstatement.cpp db/sqlstatement.h\
	db/sqlite/sqlitecnxdrv.cpp db/sqlite/sqlitecnxdrv.h \
	db/sqlite/sqlitecnxmgrdrv.cpp db/sqlite/sqlitecnxmgrdrv.h

