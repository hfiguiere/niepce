AC_INIT(niepce, 0.0.1)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR(README)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_MAINTAINER_MODE
AC_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR(m4)

MAJOR_VERSION=0
MINOR_VERSION=0
MICRO_VERSION=1

NIEPCE_VERSION="$MAJOR_VERSION.$MINOR_VERSION.$MICRO_VERSION"
AC_SUBST(NIEPCE_VERSION)


LIBGTKMM_VERSION=2.6.0
LIBGLADEMM_VERSION=2.6.0
LIBGCONFMM_VERSION=2.6.0
LIBGNOMEUI_VERSION=2.0.0
LIBGTKIMAGEVIEW_VERSION=1.5.0

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


CPPFLAGS="$CPPFLAGS $DEBUG_CFLAGS -pedantic -Wall -Wcast-align -Wcast-qual -Wpointer-arith -Wreturn-type"
CFLAGS="$CFLAGS "
CXXFLAGS="$CXXFLAGS "
dnl CFLAGS=""
LDFLAGS="$LDFLAGS"


dnl AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG_CPLUSPLUS
AC_LANG_COMPILER_REQUIRE

PKG_CHECK_MODULES(LIBGTKMM, [gtkmm-2.4 >= $LIBGTKMM_VERSION])
PKG_CHECK_MODULES(LIBGLADEMM, [libglademm-2.4 >= $LIBGLADEMM_VERSION])
dnl PKG_CHECK_MODULES(LIBGNOMEUI, [libgnomeui-2.0 >= $LIBGNOMEUI_VERSION])
PKG_CHECK_MODULES(LIBGCONFMM, [gconfmm-2.6 >= $LIBGCONFMM_VERSION])
PKG_CHECK_MODULES(GNOMEVFS, [gnome-vfs-2.0 > 2.0])
PKG_CHECK_MODULES(SQLITE3, [sqlite3 >= 3.0])
PKG_CHECK_MODULES(EXEMPI, [exempi-2.0 >= 1.99])
PKG_CHECK_MODULES(GTKIMAGEVIEW, [gtkimageview >= $LIBGTKIMAGEVIEW_VERSION])

PKG_CHECK_MODULES(OPENRAW, libopenraw-gnome-1.0)
AC_SUBST(OPENRAW_CFLAGS)
AC_SUBST(OPENRAW_LIBS)

AX_BOOST_BASE([1.33.1])
AX_BOOST_UNIT_TEST_FRAMEWORK
AX_BOOST_THREAD
dnl we need to do an extra check on the boost headers
AC_CHECK_HEADER(boost/thread.hpp, , AC_MSG_ERROR([Boost.Thread headers not found.]))
AX_BOOST_FILESYSTEM
AC_CHECK_HEADER(boost/filesystem/path.hpp, , AC_MSG_ERROR([Boost.Filesystem headers not found.]))

dnl hack since AX_BOOST_FILESYSTEM seems to fail
if test "x$BOOST_FILESYSTEM_LIB" = "x" ; then
   BOOST_FILESYSTEM_LIB=-lboost_filesystem
fi

IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=niepce
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
ALL_LINGUAS="fr"
AM_GLIB_GNU_GETTEXT

NIEPCE_LOCALEDIR=[${datadir}/locale]
AC_SUBST(NIEPCE_LOCALEDIR)

CPPFLAGS="$CPPFLAGS "

AC_CONFIG_FILES([
Makefile
doc/Doxyfile
doc/Makefile
doc/doxygen/Makefile
src/Makefile
src/ext/Makefile
src/niepce/Makefile
src/utils/Makefile
src/db/Makefile
src/db/sqlite/Makefile
src/library/Makefile
src/libraryclient/Makefile
src/framework/Makefile
src/ui/Makefile
src/ui/thumb-view/Makefile
src/main/Makefile
po/Makefile.in
po/Makefile
])


AC_OUTPUT
