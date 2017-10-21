
AM_CPPFLAGS = -I$(top_srcdir)/src/  @FRAMEWORK_CFLAGS@ \
	@GPHOTO_CFLAGS@ \
	$(NULL)

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

noinst_LIBRARIES = libniepceengine.a libniepceengineglue.a

libniepceengine_a_SOURCES = \
	db/libfile.hpp db/libfile.cpp \
	db/libfolder.hpp db/libfolder.cpp \
	db/label.hpp db/label.cpp \
	db/libmetadata.hpp db/libmetadata.cpp \
	db/storage.hpp \
	db/fsfile.hpp \
	db/metadata.hpp \
	db/properties.hpp db/properties.cpp \
	db/properties-def.hpp \
	library/clienttypes.hpp \
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
