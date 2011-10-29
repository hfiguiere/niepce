SUBDIRS = utils toolkit


INCLUDES = -I$(top_srcdir)/src \
	@BOOST_CPPFLAGS@ \
	$(NULL)

noinst_LIBRARIES = libfwk.a


TESTS = testmoniker testgeometry

check_PROGRAMS = testmoniker testgeometry

testmoniker_SOURCES = base/t/testmoniker.cpp
testmoniker_LDADD = libfwk.a \
	@LIBGLIBMM_LIBS@

testgeometry_SOURCES = base/t/testgeometry.cpp
testgeometry_LDADD = libfwk.a \
	@LIBGLIBMM_LIBS@


libfwk_a_SOURCES = base/color.hpp base/color.cpp \
	base/autoflag.hpp \
	base/debug.hpp base/debug.cpp \
	base/fractions.hpp base/fractions.cpp \
	base/moniker.hpp base/moniker.cpp \
	base/geometry.hpp base/geometry.cpp \
	base/singleton.hpp \
	base/map.hpp \
	$(NULL)
