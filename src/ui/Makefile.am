

SUBDIRS = thumb-view

gladefiles = preferences.glade

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

INCLUDES = -I$(srcdir)/.. -I$(srcdir)/thumb-view \
	-DGLADEDIR=\"$(gladedir)\" \
	@LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @LIBGCONFMM_CFLAGS@

EXTRA_DIST = $(gladefiles)

noinst_LTLIBRARIES = libniepceui.la


noinst_HEADERS = niepcewindow.h niepceapplication.h \
	librarymainview.h librarymainviewcontroller.h \
	workspacecontroller.h

libniepceui_la_CPPFLAGS = 
libniepceui_la_SOURCES = niepcewindow.cpp \
	niepceapplication.cpp librarymainview.cpp \
	librarymainviewcontroller.cpp \
	workspacecontroller.cpp \
	metadatapanecontroller.h metadatapanecontroller.cpp
