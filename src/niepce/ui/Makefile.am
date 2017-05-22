

gladefiles = dialogs/preferences.ui \
	dialogs/importdialog.ui \
	dialogs/editlabels.ui \
	dialogs/importers/directoryimporterui.ui \
	$(NULL)

gladedir = @datarootdir@/niepce/glade/
glade_DATA = $(gladefiles)

AM_CPPFLAGS = -I$(top_srcdir)/src/ -I$(srcdir)/.. \
	-I$(top_srcdir)/src/ext \
	-DGLADEDIR=\"$(gladedir)\" \
	-DDATADIR=\"$(datadir)\" \
	@FRAMEWORK_CFLAGS@

EXTRA_DIST = $(gladefiles)

noinst_LIBRARIES = libniepceui.a

PUBLICHEADERS = ilibrarymodule.hpp imoduleshell.hpp dialogs/importers/iimporterui.hpp

libniepceui_a_SOURCES = \
	niepcewindow.hpp niepcewindow.cpp \
	niepceapplication.hpp niepceapplication.cpp \
	gridviewmodule.hpp gridviewmodule.cpp \
	moduleshell.hpp moduleshell.cpp \
	moduleshellwidget.hpp moduleshellwidget.cpp \
	librarycellrenderer.hpp librarycellrenderer.cpp \
	imageliststore.hpp imageliststore.cpp\
	workspacecontroller.hpp workspacecontroller.cpp \
	metadatapanecontroller.hpp metadatapanecontroller.cpp \
	dialogs/editlabels.hpp dialogs/editlabels.cpp \
	dialogs/importdialog.hpp dialogs/importdialog.cpp \
	dialogs/preferencesdialog.hpp dialogs/preferencesdialog.cpp \
	dialogs/importdialog.hpp dialogs/importdialog.cpp \
	dialogs/importers/importerui.hpp \
	dialogs/importers/importerui.cpp \
	dialogs/importers/directoryimporterui.hpp \
	dialogs/importers/directoryimporterui.cpp \
	dialogs/importers/cameraimporterui.hpp \
	dialogs/importers/cameraimporterui.cpp \
	selectioncontroller.hpp selectioncontroller.cpp \
	filmstripcontroller.hpp filmstripcontroller.cpp \
	thumb-view/eog-thumb-nav.cpp thumb-view/eog-thumb-nav.hpp \
	thumbstripview.cpp thumbstripview.hpp \
	$(PUBLICHEADERS) \
	$(NULL)
