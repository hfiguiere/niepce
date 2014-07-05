
SUBDIRS = ui modules

AM_CPPFLAGS = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src/niepce -I$(top_srcdir)/src  \
	@EXEMPI_CFLAGS@ @LIBGLIBMM_CFLAGS@ \
	@LIBGTKMM_CFLAGS@ \
	-DNIEPCE_LOCALEDIR=\"@NIEPCE_LOCALEDIR@\"

bin_PROGRAMS = niepce

niepce_LDFLAGS =
niepce_LDADD = \
	$(top_builddir)/src/engine/db/libniepcedb.a \
	$(top_builddir)/src/niepce/ui/libniepceui.a \
	$(top_builddir)/src/engine/library/libniepcelibrary.a \
	$(top_builddir)/src/libraryclient/liblibraryclient.a \
	$(top_builddir)/src/engine/library/libniepcelibrary.a \
	$(top_builddir)/src/engine/db/libniepcedb.a \
	$(top_builddir)/src/niepce/modules/darkroom/libmoduledarkroom.a \
	$(top_builddir)/src/niepce/modules/interfaces/libinterfaces.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	$(top_builddir)/src/ncr/libncr.a \
	$(top_builddir)/src/ext/libview/libview.a \
	@LIBGLIBMM_LIBS@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ \
	@BABL_LIBS@ \
	@GEGL_LIBS@ @OPENRAW_LIBS@ @EXEMPI_LIBS@


niepce_SOURCES = \
	stock.hpp stock.cpp \
	notificationcenter.hpp notificationcenter.cpp \
        notifications.hpp \
	main.cpp
