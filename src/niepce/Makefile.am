
SUBDIRS = ui modules

INCLUDES = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src/niepce -I$(top_srcdir)/src  \
	@EXEMPI_CFLAGS@ @LIBGTKMM_CFLAGS@

bin_PROGRAMS = niepce

niepce_CPPFLAGS = @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@	@LIBGCONFMM_CFLAGS@
niepce_LDFLAGS = -Wl,--as-needed @BOOST_FILESYSTEM_LDFLAGS@ \
	@BOOST_THREAD_LDFLAGS@ @BOOST_SIGNALS_LDFLAGS@
niepce_LDADD = \
	$(top_builddir)/src/db/libniepcedb.a \
	$(top_builddir)/src/niepce/ui/libniepceui.a \
	$(top_builddir)/src/library/libniepcelibrary.a \
	$(top_builddir)/src/libraryclient/liblibraryclient.a \
	$(top_builddir)/src/library/libniepcelibrary.a \
	$(top_builddir)/src/db/libniepcedb.a \
	$(top_builddir)/src/niepce/modules/darkroom/libmoduledarkroom.a \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/ncr/libncr.a \
	$(top_builddir)/src/ext/libgdl/libgdl.a \
	@LIBGTKMM_LIBS@ @LIBGLADEMM_LIBS@ @SQLITE3_LIBS@ \
	@GNOMEVFS_LIBS@ @LIBGCONFMM_LIBS@ @BOOST_THREAD_LIBS@ \
	@BOOST_FILESYSTEM_LIBS@  @BOOST_SIGNALS_LIBS@ \
	@GOOCANVASMM_LIBS@ @BABL_LIBS@ \
	@GEGLMM_LIBS@ @OPENRAW_LIBS@ @EXEMPI_LIBS@


niepce_SOURCES = xmp.cpp \
	stock.h stock.cpp \
        notifications.h xmp.h \
	main.cpp
