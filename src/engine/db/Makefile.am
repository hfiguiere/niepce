

SUBDIRS =

AM_CPPFLAGS = -I$(top_srcdir)/src/  @FRAMEWORK_CFLAGS@

TESTS = test_library test_filebundle

TEST_LIBS =  \
	libniepcedb.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	@BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@FRAMEWORK_LIBS@

check_PROGRAMS = test_library test_filebundle

test_library_SOURCES = test_library.cpp
test_library_LDADD = $(TEST_LIBS)

test_filebundle_SOURCES = test_filebundle.cpp
test_filebundle_LDADD = $(TEST_LIBS)

noinst_LIBRARIES = libniepcedb.a

libniepcedb_a_SOURCES = library.hpp library.cpp \
	librarytypes.hpp \
	libfile.hpp libfile.cpp \
	libfolder.hpp libfolder.cpp \
	label.hpp \
	libmetadata.hpp libmetadata.cpp \
	keyword.hpp keyword.cpp \
	storage.hpp storage.cpp \
	fsfile.hpp fsfile.cpp \
	filebundle.hpp filebundle.cpp \
	metadata.hpp \
	properties.hpp properties.cpp \
	properties-def.hpp
