
AM_CPPFLAGS = -I$(top_srcdir)/src \
	@BOOST_CPPFLAGS@ \
	@FRAMEWORK_CFLAGS@ \
	$(NULL)

noinst_LIBRARIES = libfwkbase.a


TESTS = testmoniker testgeometry testfractions testdate testmap\
	testoption testpropertybag

check_PROGRAMS = testmoniker testgeometry testfractions testdate testmap\
	testoption testpropertybag

testdate_SOURCES = t/testdate.cpp
testdate_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

testmoniker_SOURCES = t/testmoniker.cpp
testmoniker_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

testgeometry_SOURCES = t/testgeometry.cpp
testgeometry_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

testfractions_SOURCES = t/testfractions.cpp
testfractions_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

testmap_SOURCES = t/testmap.cpp
testmap_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

testoption_SOURCES = t/testoption.cpp
testoption_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

testpropertybag_SOURCES = t/testpropertybag.cpp
testpropertybag_LDADD = libfwkbase.a \
	@FRAMEWORK_LIBS@

libfwkbase_a_SOURCES = colour.hpp colour.cpp \
	autoflag.hpp \
	date.hpp date.cpp \
	debug.hpp debug.cpp \
	fractions.hpp fractions.cpp \
	moniker.hpp moniker.cpp \
	geometry.hpp geometry.cpp \
	singleton.hpp \
	util.hpp \
	map.hpp \
	option.hpp \
	propertybag.hpp propertybag.cpp \
	$(NULL)
