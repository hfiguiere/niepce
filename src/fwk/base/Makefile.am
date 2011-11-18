
INCLUDES = -I$(top_srcdir)/src \
	@BOOST_CPPFLAGS@ \
	$(NULL)

noinst_LIBRARIES = libfwkbase.a


TESTS = testmoniker testgeometry

check_PROGRAMS = testmoniker testgeometry

testmoniker_SOURCES = t/testmoniker.cpp
testmoniker_LDADD = libfwkbase.a \
	@LIBGLIBMM_LIBS@

testgeometry_SOURCES = t/testgeometry.cpp
testgeometry_LDADD = libfwkbase.a \
	@LIBGLIBMM_LIBS@



libfwkbase_a_SOURCES = color.hpp color.cpp \
	autoflag.hpp \
	debug.hpp debug.cpp \
	fractions.hpp fractions.cpp \
	moniker.hpp moniker.cpp \
	geometry.hpp geometry.cpp \
	singleton.hpp \
	map.hpp \
	propertybag.hpp propertybag.cpp \
	$(NULL)
