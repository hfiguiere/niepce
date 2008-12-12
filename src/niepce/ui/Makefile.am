

DIST_SUBDIRS = thumb-view

gladefiles = preferences.glade \
	importdialog.glade	

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

INCLUDES = -I$(top_srcdir)/src/ -I$(srcdir)/.. -I$(srcdir)/thumb-view \
	-I$(top_srcdir)/src/ext \
	-DGLADEDIR=\"$(gladedir)\" \
	-DDATADIR=\"$(datadir)\" \
	@LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @LIBGCONFMM_CFLAGS@ \
	@GOOCANVASMM_CFLAGS@ \
	@EXEMPI_CFLAGS@

EXTRA_DIST = $(gladefiles)

noinst_LIBRARIES = libniepceui.a

libniepceui_a_CPPFLAGS = 
libniepceui_a_SOURCES = niepcewindow.h niepcewindow.cpp \
	niepceapplication.h niepceapplication.cpp \
	librarymainview.h librarymainview.cpp \
	librarycellrenderer.h librarycellrenderer.cpp \
	librarymainviewcontroller.h librarymainviewcontroller.cpp \
	imageliststore.h imageliststore.cpp\
	workspacecontroller.h workspacecontroller.cpp \
	metadatapanecontroller.h metadatapanecontroller.cpp \
	importdialog.h importdialog.cpp \
	selectioncontroller.h selectioncontroller.cpp \
	filmstripcontroller.h filmstripcontroller.cpp \
	thumb-view/eog-thumb-nav.cpp thumb-view/eog-thumb-nav.h \
	thumb-view/eog-thumb-view.cpp thumb-view/eog-thumb-view.h
