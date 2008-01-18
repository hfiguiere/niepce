


INCLUDES = -I$(srcdir)/.. @EXEMPI_CFLAGS@

bin_PROGRAMS = niepce


niepce_SOURCES = main.cpp

niepce_CPPFLAGS = @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@	@LIBGCONFMM_CFLAGS@
niepce_LDFLAGS = -Wl,--as-needed @BOOST_FILESYSTEM_LDFLAGS@ \
	@BOOST_THREAD_LDFLAGS@
niepce_LDADD = \
	$(top_builddir)/src/db/libniepcedb.la \
	$(top_builddir)/src/db/sqlite/libniepcedbsqlite.la \
	$(top_builddir)/src/ui/libniepceui.la \
	$(top_builddir)/src/ui/thumb-view/libthumbview.la \
	$(top_builddir)/src/library/libniepcelibrary.la \
	$(top_builddir)/src/libraryclient/liblibraryclient.la \
	$(top_builddir)/src/library/libniepcelibrary.la \
	$(top_builddir)/src/db/libniepcedb.la \
	$(top_builddir)/src/db/sqlite/libniepcedbsqlite.la \
	$(top_builddir)/src/framework/libniepceframework.la \
	$(top_builddir)/src/utils/libniepceutils.la \
	$(top_builddir)/src/niepce/libniepceglobals.la \
	@LIBGTKMM_LIBS@ @LIBGLADEMM_LIBS@ @SQLITE3_LIBS@ @GTKIMAGEVIEW_LIBS@ \
	@GNOMEVFS_LIBS@ @LIBGCONFMM_LIBS@ @BOOST_THREAD_LIBS@ \
	@BOOST_FILESYSTEM_LIBS@  \
	@OPENRAW_LIBS@ @EXEMPI_LIBS@
