

AM_CPPFLAGS = -I$(top_srcdir)/src/  @FRAMEWORK_CFLAGS@ \
	@GPHOTO_CFLAGS@ \
	$(NULL)

TESTS = test_opqueue

TEST_LIBS =  \
	libniepceengine.a \
	$(top_builddir)/target/debug/libniepce_rust.a \
	libniepceengineglue.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	@BOOST_UNIT_TEST_FRAMEWORK_LIBS@ \
	@FRAMEWORK_LIBS@ \
	@OPENRAW_LIBS@ \
	-ldl \
	$(NULL)


check_PROGRAMS = test_opqueue

test_opqueue_SOURCES = library/test_opqueue.cpp library/opqueue.hpp
test_opqueue_LDADD = $(TEST_LIBS)

noinst_LIBRARIES = libniepceengine.a libniepceengineglue.a

libniepceengine_a_SOURCES = \
	db/library.hpp db/library.cpp \
	db/libfile.hpp db/libfile.cpp \
	db/libfolder.hpp db/libfolder.cpp \
	db/label.hpp db/label.cpp \
	db/libmetadata.hpp db/libmetadata.cpp \
	db/keyword.hpp db/keyword.cpp \
	db/storage.hpp \
	db/fsfile.hpp \
	db/metadata.hpp \
	db/properties.hpp db/properties.cpp \
	db/properties-def.hpp \
	library/clienttypes.hpp \
	library/op.hpp library/op.cpp  \
	library/thumbnailcache.hpp library/thumbnailcache.cpp \
	library/thumbnailnotification.hpp \
	importer/iimporter.hpp \
	importer/directoryimporter.hpp \
	importer/directoryimporter.cpp \
	importer/cameraimporter.hpp \
	importer/cameraimporter.cpp \
	importer/importedfile.hpp \
	importer/importedfile.cpp \
	$(NULL)

# glue code called from Rust. Library order issue.
libniepceengineglue_a_SOURCES = \
	library/notification.hpp library/notification.cpp \
	$(NULL)
