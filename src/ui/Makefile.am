
INCLUDES = -I$(srcdir)/..

gladefiles = mainwindow.glade

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

EXTRA_DIST = $(gladefiles)

noinst_LTLIBRARIES = libniepceui.la


noinst_HEADERS = niepcewindow.h niepceapplication.h

libniepceui_la_CPPFLAGS = @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	-DGLADEDIR=\"$(gladedir)\"
libniepceui_la_SOURCES = niepcewindow.cpp \
	niepceapplication.cpp