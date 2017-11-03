
SUBDIRS = ui modules

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src/niepce -I$(top_srcdir)/src  \
	@FRAMEWORK_CFLAGS@ \
	-DNIEPCE_LOCALEDIR=\"@NIEPCE_LOCALEDIR@\"

bin_PROGRAMS = niepce

niepce_LDFLAGS =
niepce_LDADD = \
	$(top_builddir)/src/niepce/ui/libniepceui.a \
	$(top_builddir)/src/libraryclient/liblibraryclient.a \
	$(top_builddir)/src/engine/libniepceengine.a \
	$(top_builddir)/src/niepce/modules/darkroom/libmoduledarkroom.a \
	$(top_builddir)/src/niepce/modules/map/libmodulemap.a \
	$(top_builddir)/src/niepce/modules/interfaces/libinterfaces.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	$(top_builddir)/src/ncr/libncr.a \
	$(top_builddir)/src/ext/libview/libview.a \
	$(top_builddir)/target/@CARGO_TARGET_SUBDIR@/libniepce_rust.a \
	$(top_builddir)/src/engine/libniepceengine.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/engine/libniepceengineglue.a \
	@FRAMEWORK_LIBS@ \
	@GPHOTO_LIBS@ \
	@BABL_LIBS@ \
	@GEGL_LIBS@ @OPENRAW_LIBS@ \
	-ldl \
	$(NULL)


niepce_SOURCES = \
	notificationcenter.hpp notificationcenter.cpp \
        notifications.hpp \
	main.cpp
