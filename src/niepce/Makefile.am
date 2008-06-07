

INCLUDES = -DDATADIR=\"$(datadir)\" \
	-I$(srcdir)/.. @EXEMPI_CFLAGS@ @LIBGTKMM_CFLAGS@

noinst_HEADERS = notifications.h xmp.h

noinst_LIBRARIES = libniepceglobals.a

libniepceglobals_a_SOURCES = xmp.cpp \
	stock.h stock.cpp
