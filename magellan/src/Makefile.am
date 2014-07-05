

AM_CPPFLAGS = -DMG_LOCALEDIR=\"${MG_LOCALEDIR}\" \
	-DDATADIR=\"$(datadir)\" \
	-DGLADEDIR=\"$(gladedir)\" \
	-I$(top_srcdir)/src  \
	@LIBGLIBMM_CFLAGS@ @LIBGTKMM_CFLAGS@

gladefiles = mgwindow.ui
gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

bin_PROGRAMS = magellan

magellan_SOURCES = \
	mgapplication.hpp mgapplication.cpp \
	mgwindow.hpp mgwindow.cpp \
	main.cpp


magellan_LDADD = @LIBGLIBMM_LIBS@ @LIBGTKMM_LIBS@ \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	$(NULL)
