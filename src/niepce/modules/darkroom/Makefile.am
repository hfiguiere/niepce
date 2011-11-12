

INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/niepce -I$(top_srcdir)/src/ext \
	-DGLADEDIR=\"$(gladedir)\" \
	-DDATADIR=\"$(datadir)\" \
	@LIBGLIBMM_CFLAGS@ \
	@LIBGTKMM_CFLAGS@ \
	@GEGLMM_CFLAGS@ @EXEMPI_CFLAGS@ @OPENRAW_CFLAGS@


noinst_LIBRARIES=libmoduledarkroom.a


libmoduledarkroom_a_SOURCES = darkroommodule.cpp darkroommodule.hpp \
	imagecanvas.hpp imagecanvas.cpp \
	toolboxcontroller.hpp toolboxcontroller.cpp \
	dritemwidget.hpp dritemwidget.cpp \
	$(NULL)
