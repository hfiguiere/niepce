

INCLUDES = -I$(top_srcdir)/src  \
	-DGLADEDIR=\"$(gladedir)\" \
	-DDATADIR=\"$(datadir)\" \
	@LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @LIBGCONFMM_CFLAGS@ \
	@GOOCANVASMM_CFLAGS@ \
	@GEGLMM_CFLAGS@ @EXEMPI_CFLAGS@


noinst_LIBRARIES=libmoduledarkroom.a


libmoduledarkroom_a_SOURCES = darkroommodule.cpp darkroommodule.h \
	image.h image.cpp \
	imagecanvas.h imagecanvas.cpp
