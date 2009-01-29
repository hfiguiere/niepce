

INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/niepce -I$(top_srcdir)/src/ext \
	-DGLADEDIR=\"$(gladedir)\" \
	-DDATADIR=\"$(datadir)\" \
	@LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @GCONF_CFLAGS@ \
	@GOOCANVASMM_CFLAGS@  \
	@GEGLMM_CFLAGS@ @EXEMPI_CFLAGS@ @OPENRAW_CFLAGS@


noinst_LIBRARIES=libmoduledarkroom.a


libmoduledarkroom_a_SOURCES = darkroommodule.cpp darkroommodule.h \
	imagecanvas.h imagecanvas.cpp \
	toolboxcontroller.h toolboxcontroller.cpp \
	dritemwidget.h dritemwidget.cpp \
	$(NULL)
