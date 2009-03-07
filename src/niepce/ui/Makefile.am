

DIST_SUBDIRS = thumb-view dialogs

gladefiles = dialogs/preferences.ui \
	dialogs/importdialog.ui

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

INCLUDES = -I$(top_srcdir)/src/ -I$(srcdir)/.. -I$(srcdir)/thumb-view \
	-I$(top_srcdir)/src/ext \
	-DGLADEDIR=\"$(gladedir)\" \
	-DDATADIR=\"$(datadir)\" \
	@LIBGLIBMM_CFLAGS@ \
	@LIBGTKMM_CFLAGS@ \
	@GNOMEVFS_CFLAGS@ @GCONF_CFLAGS@ \
	@GOOCANVASMM_CFLAGS@ \
	@EXEMPI_CFLAGS@

EXTRA_DIST = $(gladefiles)

noinst_LIBRARIES = libniepceui.a

libniepceui_a_CPPFLAGS = 
libniepceui_a_SOURCES = niepcewindow.hpp niepcewindow.cpp \
	niepceapplication.hpp niepceapplication.cpp \
	librarymainview.h librarymainview.cpp \
	librarycellrenderer.h librarycellrenderer.cpp \
	librarymainviewcontroller.h librarymainviewcontroller.cpp \
	imageliststore.h imageliststore.cpp\
	workspacecontroller.h workspacecontroller.cpp \
	metadatapanecontroller.hpp metadatapanecontroller.cpp \
	dialogs/importdialog.hpp dialogs/importdialog.cpp \
	dialogs/preferencesdialog.hpp dialogs/preferencesdialog.cpp \
	selectioncontroller.h selectioncontroller.cpp \
	filmstripcontroller.h filmstripcontroller.cpp \
	dialogs/importdialog.hpp dialogs/importdialog.cpp \
	thumb-view/eog-thumb-nav.cpp thumb-view/eog-thumb-nav.h \
	thumb-view/eog-thumb-view.cpp thumb-view/eog-thumb-view.h
