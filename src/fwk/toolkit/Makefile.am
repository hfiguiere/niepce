


INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/ext \
	@LIBGLIBMM_CFLAGS@ \
	@LIBGTKMM_CFLAGS@ \
	@GCONF_CFLAGS@  @GNOMEVFS_CFLAGS@ @EXEMPI_CFLAGS@ \
	$(NULL)

noinst_LIBRARIES = libniepceframework.a

noinst_HEADERS =   \
	  goocanvas_proxy_header.hpp

libniepceframework_a_SOURCES = configuration.hpp configuration.cpp \
	application.hpp application.cpp \
	frame.hpp frame.cpp \
	controller.hpp controller.cpp \
	notification.hpp \
	mimetype.hpp mimetype.cpp \
	imageloader.hpp imageloader.cpp \
	notificationcenter.hpp notificationcenter.cpp \
	configdatabinder.hpp configdatabinder.cpp \
	gdkutils.hpp gdkutils.cpp \
	gtkutils.hpp gtkutils.cpp \
	widgets/toolboxitemwidget.hpp widgets/toolboxitemwidget.cpp \
	widgets/editablehscale.hpp widgets/editablehscale.cpp \
	widgets/dock-item.cpp widgets/dock-item.hpp \
	widgets/dock.cpp widgets/dock.hpp \
	dockable.hpp dockable.cpp \
	metadatawidget.hpp metadatawidget.cpp \
	undo.hpp undo.cpp \
	command.hpp \
	$(NULL)
