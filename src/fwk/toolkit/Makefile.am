


INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/ext \
	@LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@GCONF_CFLAGS@  @GNOMEVFS_CFLAGS@ @EXEMPI_CFLAGS@ \
	$(NULL)

noinst_LIBRARIES = libniepceframework.a

noinst_HEADERS = configuration.h application.h \
	frame.h controller.h goocanvas_proxy_header.h \
	gconf_proxy_header.h

libniepceframework_a_SOURCES = configuration.cpp \
	application.cpp frame.cpp controller.cpp \
	notification.h \
	mimetype.h mimetype.cpp \
	imageloader.h imageloader.cpp \
	notificationcenter.h notificationcenter.cpp \
	configdatabinder.h configdatabinder.cpp \
	gdkutils.h gdkutils.cpp \
	widgets/toolboxitemwidget.h widgets/toolboxitemwidget.cpp \
	widgets/editablehscale.h widgets/editablehscale.cpp \
	widgets/dock-item.cpp widgets/dock-item.h \
	widgets/dock.cpp widgets/dock.h \
	dockable.h dockable.cpp \
	metadatawidget.h metadatawidget.cpp \
	undo.h undo.cpp \
	command.h \
	$(NULL)
