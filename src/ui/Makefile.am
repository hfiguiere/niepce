

SUBDIRS = thumb-view

gladefiles = preferences.glade \
	importdialog.glade	

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

INCLUDES = -I$(srcdir)/.. -I$(srcdir)/thumb-view \
	-DGLADEDIR=\"$(gladedir)\" \
	@LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @LIBGCONFMM_CFLAGS@

EXTRA_DIST = $(gladefiles)

noinst_LTLIBRARIES = libniepceui.la

libniepceui_la_CPPFLAGS = 
libniepceui_la_SOURCES = niepcewindow.h niepcewindow.cpp \
	niepceapplication.h niepceapplication.cpp \
	librarymainview.h librarymainview.cpp \
	librarymainviewcontroller.h librarymainviewcontroller.cpp \
	workspacecontroller.h workspacecontroller.cpp \
	metadatapanecontroller.h metadatapanecontroller.cpp \
	importdialog.h importdialog.cpp
