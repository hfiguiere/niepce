
SUBDIRS = ui modules

INCLUDES = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src/niepce -I$(top_srcdir)/src  \
	@EXEMPI_CFLAGS@ @LIBGLIBMM_CFLAGS@

bin_PROGRAMS = niepce

niepce_CPPFLAGS = @LIBGTKMM_CFLAGS@ @GCONF_CFLAGS@ \
	-DNIEPCE_LOCALEDIR=\"@NIEPCE_LOCALEDIR@\"

niepce_LDFLAGS =
niepce_LDADD = \
	$(top_builddir)/src/engine/db/libniepcedb.a \
	$(top_builddir)/src/niepce/ui/libniepceui.a \
	$(top_builddir)/src/engine/library/libniepcelibrary.a \
	$(top_builddir)/src/libraryclient/liblibraryclient.a \
	$(top_builddir)/src/engine/library/libniepcelibrary.a \
	$(top_builddir)/src/engine/db/libniepcedb.a \
	$(top_builddir)/src/niepce/modules/darkroom/libmoduledarkroom.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/libfwk.a \
	$(top_builddir)/src/ncr/libncr.a \
	$(top_builddir)/src/ext/libgdl/libgdl.a \
	$(top_builddir)/src/ext/libview/libview.a \
	@LIBGLIBMM_LIBS@ \
	@LIBGTKMM_LIBS@ @SQLITE3_LIBS@ \
	@GCONF_LIBS@ @BABL_LIBS@ \
	@GEGLMM_LIBS@ @OPENRAW_LIBS@ @EXEMPI_LIBS@


niepce_SOURCES = xmp.cpp \
	stock.hpp stock.cpp \
	notificationcenter.hpp notificationcenter.cpp \
        notifications.hpp xmp.hpp \
	main.cpp
