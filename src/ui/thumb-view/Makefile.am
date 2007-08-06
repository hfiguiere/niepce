

INCLUDES = -I$(srcdir) -I$(top_srcdir)/src

noinst_LTLIBRARIES = libthumbview.la

noinst_HEADERS = eog-thumb-nav.h eog-thumb-view.h

libthumbview_la_CPPFLAGS =  @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@

libthumbview_la_SOURCES = eog-thumb-nav.c eog-thumb-view.cpp \
	eog-list-store.cpp

