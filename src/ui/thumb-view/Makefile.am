

INCLUDES = -I$(srcdir) -I$(top_srcdir)/src

noinst_LTLIBRARIES = libthumbview.la

noinst_HEADERS = eog-thumb-nav.h eog-thumb-view.h eog-list-store.h

libthumbview_la_CPPFLAGS =  @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@

SOURCES = eog-thumb-nav.cpp eog-thumb-view.cpp \
	eog-list-store.cpp

