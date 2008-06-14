


INCLUDES = -I$(top_srcdir)/src @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@LIBGCONFMM_CFLAGS@  @GNOMEVFS_CFLAGS@ @EXEMPI_CFLAGS@

noinst_LIBRARIES = libniepceframework.a

noinst_HEADERS = configuration.h application.h \
	frame.h controller.h

libniepceframework_a_SOURCES = configuration.cpp \
	application.cpp frame.cpp controller.cpp \
	notification.h \
	mimetype.h mimetype.cpp \
	imageloader.h imageloader.cpp \
	notificationcenter.h notificationcenter.cpp \
	configdatabinder.h configdatabinder.cpp \
	gdkutils.h gdkutils.cpp \
	metadatawidget.h metadatawidget.cpp \
	undo.h undo.cpp \
	command.h
