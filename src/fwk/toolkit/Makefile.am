



INCLUDES = -I$(top_srcdir)/src -I$(top_srcdir)/src/ext \
	-DDATADIR=\"$(datadir)\" \
	@LIBGLIBMM_CFLAGS@ \
	@LIBGTKMM_CFLAGS@ \
	@GCONF_CFLAGS@  @EXEMPI_CFLAGS@ \
	$(NULL)

noinst_LIBRARIES = libniepceframework.a

noinst_HEADERS =   \
	  goocanvas_proxy_header.hpp

libniepceframework_a_SOURCES = configuration.hpp configuration.cpp \
	application.hpp application.cpp \
	dialog.hpp dialog.cpp \
	frame.hpp frame.cpp \
	controller.hpp controller.cpp \
	uicontroller.hpp uicontroller.cpp \
	notification.hpp \
	mimetype.hpp mimetype.cpp \
	movieutils.hpp movieutils.cpp \
	imageloader.hpp imageloader.cpp \
	notificationcenter.hpp notificationcenter.cpp \
	configdatabinder.hpp configdatabinder.cpp \
	gdkutils.hpp gdkutils.cpp \
	gtkutils.hpp gtkutils.cpp \
	widgets/addinstreemodel.hpp widgets/addinstreemodel.cpp \
	widgets/ratingaction.hpp widgets/ratingaction.cpp \
	widgets/ratinglabel.hpp widgets/ratinglabel.cpp \
	widgets/ratingmenuitem.hpp \
	widgets/toolboxitemwidget.hpp widgets/toolboxitemwidget.cpp \
	widgets/editablehscale.hpp widgets/editablehscale.cpp \
	widgets/dock.cpp widgets/dock.hpp \
	widgets/notabtextview.hpp widgets/notabtextview.cpp \
	dockable.hpp dockable.cpp \
	metadatawidget.hpp metadatawidget.cpp \
	undo.hpp undo.cpp \
	command.hpp \
	$(NULL)
