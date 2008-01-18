


INCLUDES = -I$(top_srcdir)/src

noinst_LTLIBRARIES = libniepceframework.la

noinst_HEADERS = configuration.h application.h \
	frame.h controller.h

libniepceframework_la_CPPFLAGS = @LIBGTKMM_CFLAGS@ @LIBGLADEMM_CFLAGS@ \
	@LIBGCONFMM_CFLAGS@  @GNOMEVFS_CFLAGS@
libniepceframework_la_SOURCES = configuration.cpp \
	application.cpp frame.cpp controller.cpp \
	notification.h \
	mimetype.h mimetype.cpp \
	notificationcenter.h notificationcenter.cpp \
	configdatabinder.h configdatabinder.cpp \
	gdkutils.h gdkutils.cpp
