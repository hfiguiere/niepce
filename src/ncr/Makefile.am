


INCLUDES = -I$(top_srcdir)/src  \
	@LIBGTKMM_CFLAGS@ \
	@GEGLMM_CFLAGS@ @OPENRAW_CFLAGS@

niepcelibdir = @libdir@/niepce/
#niepcelib_LTLIBRARIES = libncr.la
noinst_LIBRARIES = libncr.a


libncr_a_SOURCES = ncr.hpp ncr.cpp \
	image.hpp image.cpp \
	init.hpp init.cpp \
	$(NULL)

#libncr_la_SOURCES = ncr.h ncr.cpp
#libncr_la_LIBADD = @OPENRAW_LIBS@ @GEGLMM_LIBS@
