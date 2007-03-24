


INCLUDES = -I$(srcdir)/..

bin_PROGRAMS = niepce


niepce_SOURCES = main.cpp

niepce_LDADD = $(top_builddir)/src/db/libniepcedb.la \
	$(top_builddir)/src/framework/libniepceframework.la \
	$(top_builddir)/src/ui/libniepceui.la \
	@LIBGTKMM_LIBS@ @LIBGLADEMM_LIBS@