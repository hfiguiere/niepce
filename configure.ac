AC_INIT(niepce, 0.0.1)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR(README)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
m4_ifndef([AM_SILENT_RULES], [m4_define([AM_SILENT_RULES],[])])
AM_SILENT_RULES([yes])
AC_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR(m4)

m4_pattern_allow([^BOOST_])

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

NIEPCE_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
AC_SUBST(NIEPCE_VERSION)


dnl all the library version.
dnl if one is harcoded elsewhere, it is a bug
LIBGIOMM_VERSION=2.16
LIBGLIBMM_VERSION=2.4
LIBGTKMM_VERSION=3.2
LIBGNOMEUI_VERSION=2.0.0
EXEMPI_VERSION=2.2.0
SQLITE_VERSION=3.0
GEGL_VERSION=0.1.0
GEGLMM_VERSION=0.2.0
LIBOPENRAW_VERSION=0.0.5
LIBGPHOTO_VERSION=2.4
dnl need at least 2.5.0 because of xmlTextReader
LIBXML2_VERSION=2.5.0
BOOST_VERSION=1.34

AC_PROG_CXX
AC_GNU_SOURCE

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


CPPFLAGS="$CPPFLAGS $DEBUG_CFLAGS -Wall -Wcast-align -Wcast-qual -Wpointer-arith -Wreturn-type"
CFLAGS="$CFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
CXXFLAGS="$CXXFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
dnl CFLAGS=""
LDFLAGS="$LDFLAGS"


dnl AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG_CPLUSPLUS
AC_LANG_COMPILER_REQUIRE

PKG_CHECK_MODULES(LIBGLIBMM, [glibmm-2.4 >= $LIBGLIBMM_VERSION giomm-2.4 >= $LIBGIOMM_VERSION gthread-2.0])
PKG_CHECK_MODULES(LIBGTKMM, [gtkmm-3.0 >= $LIBGTKMM_VERSION])
PKG_CHECK_MODULES(SQLITE3, [sqlite3 >= $SQLITE_VERSION])
PKG_CHECK_MODULES(EXEMPI, [exempi-2.0 >= $EXEMPI_VERSION])
PKG_CHECK_MODULES(LIBXML2, [libxml-2.0 >= $LIBXML2_VERSION])
PKG_CHECK_MODULES(BABL, babl)
PKG_CHECK_MODULES(GPHOTO, [libgphoto2 >= $LIBGPHOTO_VERSION libgphoto2_port])

PKG_CHECK_MODULES(OPENRAW, libopenraw-gnome-1.0 >= $LIBOPENRAW_VERSION)
AC_SUBST(OPENRAW_CFLAGS)
AC_SUBST(OPENRAW_LIBS)

PKG_CHECK_MODULES(GEGL, gegl-0.2 >= $GEGL_VERSION)
dnl we want to use geglmm.
PKG_CHECK_MODULES(GEGLMM, geglmm-0.2 >= $GEGLMM_VERSION)
AC_SUBST(GEGLMM_CFLAGS)
AC_SUBST(GEGLMM_LIBS)


BOOST_REQUIRE([$BOOST_VERSION])
BOOST_BIND
BOOST_CONVERSION
BOOST_FORMAT
BOOST_FUNCTION
BOOST_SMART_PTR
BOOST_TEST([s])

AC_LANG_PUSH(C++)
dnl -Wmissing-noreturn -Wshadow
if test "$GCC" = "yes"; then
        NIEPCE_BUILD_CONFIG="$NIEPCE_BUILD_CONFIG gcc-options="
        for option in -Wall -Wextra -Wsign-compare -Wpointer-arith \
                      -Wchar-subscripts -Wwrite-strings \
                      -Wunused -Wpointer-arith -fshow-column ; do
                SAVE_CXXFLAGS="$CXXFLAGS"
                CXXFLAGS="$CXXFLAGS $option"
		NIEPCE_BUILD_CONFIG="$NIEPCE_BUILD_CONFIG$option "
                AC_MSG_CHECKING([whether gcc understands $option])
                AC_TRY_COMPILE([], [],
                        has_option=yes,
                        has_option=no,)
                if test $has_option = no; then
                  CXXFLAGS="$SAVE_CXXFLAGS"
                fi
                AC_MSG_RESULT($has_option)
                unset has_option
                unset SAVE_CXXFLAGS
        done
        unset option
fi
AC_LANG_POP


IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=niepce
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

NIEPCE_LOCALEDIR=[${datadir}/locale]
AC_SUBST(NIEPCE_LOCALEDIR)

AC_DEFINE_UNQUOTED([NIEPCE_BUILD_CONFIG], ["$NIEPCE_BUILD_CONFIG"], [The string used to hardcode the build config.])

AC_CONFIG_SUBDIRS(camerawire)

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
src/niepce/ui/Makefile
src/niepce/ui/dialogs/Makefile
src/niepce/ui/thumb-view/Makefile
src/niepce/modules/Makefile
src/niepce/modules/darkroom/Makefile
src/niepce/modules/interfaces/Makefile
src/niepce/Makefile
src/fwk/Makefile
src/fwk/base/Makefile
src/fwk/utils/Makefile
src/fwk/utils/db/Makefile
src/fwk/utils/db/sqlite/Makefile
src/fwk/toolkit/Makefile
src/engine/Makefile
src/engine/db/Makefile
src/engine/library/Makefile
src/libraryclient/Makefile
src/ncr/Makefile
po/Makefile.in
po/Makefile
])


AC_OUTPUT
