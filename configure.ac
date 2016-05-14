AC_INIT(niepce-suite, 2013.6.1)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR(README)
AM_INIT_AUTOMAKE([1.11 no-dist-gzip dist-xz subdir-objects])
m4_ifndef([AM_SILENT_RULES], [m4_define([AM_SILENT_RULES],[])])
AM_SILENT_RULES([yes])
AC_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR(m4)

m4_pattern_allow([^BOOST_])

dnl all the library version.
dnl if one is harcoded elsewhere, it is a bug
LIBGIOMM_VERSION=2.16
LIBGLIBMM_VERSION=2.32
LIBGTKMM_VERSION=3.14
EXEMPI_VERSION=2.2.0
SQLITE_VERSION=3.0
GEGL_VERSION=0.3.0
LIBOPENRAW_VERSION=0.1.0
LIBGPHOTO_VERSION=2.4
dnl need at least 2.5.0 because of xmlTextReader
LIBXML2_VERSION=2.5.0
BOOST_VERSION=1.34

AC_PROG_CXX
AC_GNU_SOURCE
AX_CXX_COMPILE_STDCXX_11(noext,mandatory)

AC_ARG_ENABLE(debug,[  --enable-debug    Turn on debugging],[
        case "${enableval}" in
           yes) debug=true ;
                DEBUG_CFLAGS="-DDEBUG -g" ;
                OPTIMIZE_CFLAGS="" ;;
            no) debug=false ;
                DEBUG_CFLAGS="-DNDEBUG" ;;
             *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
        esac
],[     debug=false
        DEBUG_CFLAGS="-DNDEBUG"
])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)

NP_APPS=""

AC_ARG_ENABLE(niepce,[  --enable-niepce   Enable building Niepce (default)],[
    NP_APP_NIEPCE=yes
    if test ! -z "$NP_APPS" ; then
        NP_APPS="$NP_APPS,"
    fi
    NP_APPS="$NP_APPSniepce"
],[
    NP_APP_NIEPCE=no
])
AC_ARG_ENABLE(camerawire,[  --enable-camerawire   Enable building Camerawire],[
    NP_APP_CAMERAWIRE=yes
    if test ! -z "$NP_APPS" ; then
        NP_APPS="$NP_APPS,"
    fi
    NP_APPS="$NP_APPScamerawire"
],[
    NP_APP_CAMERAWIRE=no
])
AC_ARG_ENABLE(magellan,[  --enable-magellan   Enable building Magellan],[
    NP_APP_MAGELLAN=yes
    if test ! -z "$NP_APPS" ; then
        NP_APPS="$NP_APPS,"
    fi
    NP_APPS="$NP_APPSmagellan"
],[
    NP_APP_MAGELLAN=no
])
AC_ARG_ENABLE(allapps, [  --enable-allapps   Enable all apps],[
    NP_APP_NIEPCE=yes
    NP_APP_CAMERAWIRE=yes
    NP_APP_MAGELLAN=yes
    NP_APPS="niepce,camerawire,magellan"
],[

])

if test -z "$NP_APPS" ; then
    NP_APPS="niepce"
    NP_APP_NIEPCE=yes
fi

CPPFLAGS="$CPPFLAGS $DEBUG_CFLAGS -Wall -Wcast-align -Wcast-qual -Wpointer-arith -Wreturn-type"
CFLAGS="$CFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
CXXFLAGS="$CXXFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
dnl CFLAGS=""
LDFLAGS="$LDFLAGS"


dnl AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG_CPLUSPLUS
AC_LANG_COMPILER_REQUIRE

dnl Framework requirements.
PKG_CHECK_MODULES(FRAMEWORK, [glibmm-2.4 >= $LIBGLIBMM_VERSION giomm-2.4 >= $LIBGIOMM_VERSION gthread-2.0 gtkmm-3.0 >= $LIBGTKMM_VERSION sqlite3 >= $SQLITE_VERSION exempi-2.0 >= $EXEMPI_VERSION libxml-2.0 >= $LIBXML2_VERSION cluttermm-1.0 champlain-gtk-0.12])

dnl optional framework
PKG_CHECK_MODULES(GPHOTO, [libgphoto2 >= $LIBGPHOTO_VERSION libgphoto2_port])

dnl niepce
PKG_CHECK_MODULES(BABL, babl)
PKG_CHECK_MODULES(OPENRAW, libopenraw-gnome-0.1 >= $LIBOPENRAW_VERSION)
PKG_CHECK_MODULES(GEGL, gegl-0.3 >= $GEGL_VERSION)

BOOST_REQUIRE([$BOOST_VERSION])
BOOST_CONVERSION
BOOST_FORMAT
BOOST_TEST([s])

AX_CFLAGS_GCC_OPTION([-Wextra -Wall -Wsign-compare -Wpointer-arith -Wchar-subscripts -Wwrite-strings -Wunused -Wpointer-arith])
AX_CXXFLAGS_GCC_OPTION([-Wextra -Wall -Wsign-compare -Wpointer-arith -Wchar-subscripts -Wwrite-strings -Wunused -Wpointer-arith])

AC_ARG_ENABLE(asan,[  --enable-asan    Turn on address sanitizer],[
    AX_CFLAGS_GCC_OPTION([-fsanitize=address -fno-omit-frame-pointer])
    AX_CXXFLAGS_GCC_OPTION([-fsanitize=address -fno-omit-frame-pointer])
]
,
[

])

IT_PROG_INTLTOOL([0.35.0])

AC_CONFIG_FILES([
Makefile
art/Makefile
doc/Doxyfile
doc/Makefile
doc/doxygen/Makefile
data/Makefile
data/icons/Makefile
data/themes/Makefile
src/Makefile
src/ext/Makefile
src/ext/libview/Makefile
src/fwk/Makefile
src/fwk/base/Makefile
src/fwk/utils/Makefile
src/fwk/utils/db/Makefile
src/fwk/utils/db/sqlite/Makefile
src/fwk/toolkit/Makefile
src/engine/Makefile
src/libraryclient/Makefile
src/ncr/Makefile
po/Makefile.in
po/Makefile
])

dnl =============
dnl niepce
dnl =============
AM_CONDITIONAL(NIEPCE_BUILD, test "$NP_APP_NIEPCE" = "yes")
if test "$NP_APP_NIEPCE" = "yes" ; then

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

NIEPCE_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
AC_SUBST(NIEPCE_VERSION)

GETTEXT_PACKAGE=niepce
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

NIEPCE_LOCALEDIR=[${datadir}/locale]
AC_SUBST(NIEPCE_LOCALEDIR)

AC_DEFINE_UNQUOTED([NIEPCE_BUILD_CONFIG], ["$NIEPCE_BUILD_CONFIG"], [The string used to hardcode the build config.])

AC_CONFIG_FILES([
src/niepce/ui/Makefile
src/niepce/modules/Makefile
src/niepce/modules/darkroom/Makefile
src/niepce/modules/interfaces/Makefile
src/niepce/modules/map/Makefile
src/niepce/Makefile
data/niepce.desktop.in
])

fi

dnl ================
dnl camerawire
dnl ================

AM_CONDITIONAL(CW_BUILD, test "$NP_APP_CAMERAWIRE" = "yes")
if test "$NP_APP_CAMERAWIRE" = "yes" ; then

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

CAMERAWIRE_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
AC_SUBST(CAMERAWIRE_VERSION)

CW_GETTEXT_PACKAGE=camerawire
AC_SUBST(CW_GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([CW_GETTEXT_PACKAGE], ["$CW_GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

CW_LOCALEDIR=[${datadir}/locale]
AC_SUBST(CW_LOCALEDIR)

AC_DEFINE_UNQUOTED([CW_BUILD_CONFIG], ["$CW_BUILD_CONFIG"], [The string used to hardcode the build config.])

AC_CONFIG_FILES([
camerawire/Makefile
camerawire/src/Makefile
camerawire/po/Makefile.in
camerawire/po/Makefile
])

fi

dnl ================
dnl magellan
dnl ================

AM_CONDITIONAL(MG_BUILD, test "$NP_APP_MAGELLAN" = "yes")
if test "$NP_APP_MAGELLAN" = "yes" ; then
MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

MG_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
AC_SUBST(MG_VERSION)

MG_GETTEXT_PACKAGE=magellan
AC_SUBST(MG_GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([MG_GETTEXT_PACKAGE], ["$MG_GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

MG_LOCALEDIR=[${datadir}/locale]
AC_SUBST(MG_LOCALEDIR)

AC_DEFINE_UNQUOTED([MG_BUILD_CONFIG], ["$MG_BUILD_CONFIG"], [The string used to hardcode the build config.])

AC_CONFIG_FILES([
magellan/Makefile
magellan/src/Makefile
magellan/po/Makefile.in
magellan/po/Makefile
])

fi

AC_OUTPUT

echo "

  Build apps: ${NP_APPS}

"
