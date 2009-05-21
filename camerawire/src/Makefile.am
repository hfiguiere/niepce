

AM_CPPFLAGS = -DCW_LOCALEDIR=\"${CW_LOCALEDIR}\"


bin_PROGRAMS = camerawire

INCLUDES = -DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/../src/ -I$(top_srcdir)/src  \
	@LIBGLIBMM_CFLAGS@ @LIBGTKMM_CFLAGS@ @GCONF_CFLAGS@ 


camerawire_SOURCES = \
	cwapplication.hpp cwapplication.cpp \
	main.cpp

camerawire_LDADD = @LIBGLIBMM_LIBS@ @LIBGTKMM_LIBS@ \
	@GCONF_LIBS@ \
	$(top_builddir)/../src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/../src/fwk/utils/libniepceutils.a \
	$(top_builddir)/../src/fwk/libfwk.a \
	$(NULL)
