

AM_CPPFLAGS = -DCW_LOCALEDIR=\"${CW_LOCALEDIR}\"


bin_PROGRAMS = magellan

INCLUDES = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/../src/ -I$(top_srcdir)/src  \
	@LIBGLIBMM_CFLAGS@ @LIBGTKMM_CFLAGS@

magellan_SOURCES = \
	mgapplication.hpp mgapplication.cpp \
	mgwindow.hpp mgwindow.cpp \
	main.cpp


magellan_LDADD = @LIBGLIBMM_LIBS@ @LIBGTKMM_LIBS@ \
	$(top_builddir)/../src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/../src/fwk/utils/libniepceutils.a \
	$(top_builddir)/../src/fwk/base/libfwkbase.a \
	$(NULL)
