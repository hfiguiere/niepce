

SUBDIRS = 

INCLUDES = -I$(top_srcdir)/src/  @EXEMPI_CFLAGS@

TESTS = test_library test_filebundle

TEST_LIBS =  \
	libniepcedb.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/libfwk.a \
	@LIBGTKMM_LIBS@ @GNOMEVFS_LIBS@ \
	@BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@BOOST_FILESYSTEM_LIBS@ \
	@BOOST_THREAD_LIBS@ \
	@SQLITE3_LIBS@ @EXEMPI_LIBS@

check_PROGRAMS = test_library test_filebundle

test_library_SOURCES = test_library.cpp
test_library_LDADD = $(TEST_LIBS)  

test_filebundle_SOURCES = test_filebundle.cpp
test_filebundle_LDADD = $(TEST_LIBS)  

noinst_LIBRARIES = libniepcedb.a

libniepcedb_a_SOURCES = library.hpp library.cpp \
	libfile.h libfile.cpp \
	libfolder.h \
	label.hpp \
	libmetadata.h libmetadata.cpp \
	keyword.h keyword.cpp \
	storage.h storage.cpp \
	fsfile.h fsfile.cpp \
	filebundle.hpp filebundle.cpp \
	metadata.h
