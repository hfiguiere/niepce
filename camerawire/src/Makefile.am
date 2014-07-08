

AM_CPPFLAGS = -DCW_LOCALEDIR=\"${CW_LOCALEDIR}\" \
	-DDATADIR=\"$(datadir)\" \
	-I$(top_srcdir)/src  \
	@FRAMEWORK_CFLAGS@ \
	@GPHOTO_CFLAGS@

bin_PROGRAMS = camerawire

camerawire_SOURCES = \
	cwapplication.hpp cwapplication.cpp \
	cwwindow.hpp cwwindow.cpp \
	main.cpp

camerawire_LDADD = @FRAMEWORK_LIBS@ \
	@GPHOTO_LIBS@ \
	$(top_builddir)/src/fwk/toolkit/libniepceframework.a \
	$(top_builddir)/src/fwk/utils/libniepceutils.a \
	$(top_builddir)/src/fwk/base/libfwkbase.a \
	$(NULL)
