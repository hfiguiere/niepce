
INCLUDES = -I$(srcdir)/.. -I$(srcdir)/thumb-view

SUBDIRS = thumb-view

gladefiles = mainwindow.glade

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

EXTRA_DIST = $(gladefiles)

noinst_LTLIBRARIES = libniepceui.la


noinst_HEADERS = niepcewindow.h niepceapplication.h \
	librarymainview.h librarymainviewcontroller.h \
	workspacecontroller.h

libniepceui_la_CPPFLAGS = @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @LIBGCONFMM_CFLAGS@ \
	-DGLADEDIR=\"$(gladedir)\"
libniepceui_la_SOURCES = niepcewindow.cpp \
	niepceapplication.cpp librarymainview.cpp \
	librarymainviewcontroller.cpp \
	workspacecontroller.cpp