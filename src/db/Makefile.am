

SUBDIRS = 

INCLUDES = -I$(top_srcdir)/src/  @EXEMPI_CFLAGS@

TESTS = test_library

TEST_LIBS =  \
	libniepcedb.la \
	../utils/libniepceutils.la \
	../framework/libniepceframework.la \
	@LIBGTKMM_LIBS@ @GNOMEVFS_LIBS@ \
	@BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@BOOST_FILESYSTEM_LIBS@ \
	@BOOST_THREAD_LIBS@ \
	@SQLITE3_LIBS@ @EXEMPI_LIBS@

check_PROGRAMS = test_library

test_library_SOURCES = test_library.cpp
test_library_LDADD = $(TEST_LIBS)  

noinst_LTLIBRARIES = libniepcedb.la

noinst_HEADERS = library.h 

libniepcedb_la_SOURCES = library.cpp \
	libfile.h libfile.cpp \
	libmetadata.h libmetadata.cpp \
	keyword.h keyword.cpp \
	storage.h storage.cpp
