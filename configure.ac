AC_INIT(niepce, 0.0.1)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR(README)
AM_INIT_AUTOMAKE(AC_PACKAGE_NAME, AC_PACKAGE_VERSION)
AM_MAINTAINER_MODE
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
LIBGLIBMM_VERSION=2.0
LIBGTKMM_VERSION=2.12.0
dnl LIBGLADEMM_VERSION=2.6.0
dnl LIBGCONFMM_VERSION=2.6.0
LIBGCONF_VERSION=2.0.0
LIBGNOMEUI_VERSION=2.0.0
LIBGOOCANVASMM_VERSION=0.6.0
EXEMPI_VERSION=2.0.0
SQLITE_VERSION=3.0
GEGL_VERSION=0.0.22
GEGLMM_VERSION=0.0.22
LIBOPENRAW_VERSION=0.0.5
dnl need at least 2.5.0 because of xmlTextReader
LIBXML2_VERSION=2.5.0
BOOST_VERSION=1.33.1

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
CFLAGS="$CFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
CXXFLAGS="$CXXFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
dnl CFLAGS=""
LDFLAGS="$LDFLAGS"


dnl AC_PROG_INSTALL
AC_PROG_LIBTOOL

AC_LANG_CPLUSPLUS
AC_LANG_COMPILER_REQUIRE

PKG_CHECK_MODULES(LIBGLIBMM, [glibmm-2.4])
PKG_CHECK_MODULES(LIBGTKMM, [gtkmm-2.4 >= $LIBGTKMM_VERSION])
dnl PKG_CHECK_MODULES(LIBGLADEMM, [libglademm-2.4 >= $LIBGLADEMM_VERSION])
dnl PKG_CHECK_MODULES(LIBGNOMEUI, [libgnomeui-2.0 >= $LIBGNOMEUI_VERSION])
dnl PKG_CHECK_MODULES(LIBGCONFMM, [gconfmm-2.6 >= $LIBGCONFMM_VERSION])
PKG_CHECK_MODULES(GCONF, [gconf-2.0 >= $LIBGCONF_VERSION])
PKG_CHECK_MODULES(GNOMEVFS, [gnome-vfs-2.0 >= 2.12])
PKG_CHECK_EXISTS([gnome-vfs-2.0 >= 2.14], [HAVE_GNOME_VFS_2_14=1], [HAVE_GNOME_VFS_2_14=0])
dnl AC_SUBST(GNOMEVFS_VER)
PKG_CHECK_MODULES(SQLITE3, [sqlite3 >= $SQLITE_VERSION])
PKG_CHECK_MODULES(EXEMPI, [exempi-2.0 >= $EXEMPI_VERSION])
PKG_CHECK_MODULES(LIBXML2, [libxml-2.0 >= $LIBXML2_VERSION])
PKG_CHECK_MODULES(BABL, babl)

PKG_CHECK_MODULES(OPENRAW, libopenraw-gnome-1.0 >= $LIBOPENRAW_VERSION)
AC_SUBST(OPENRAW_CFLAGS)
AC_SUBST(OPENRAW_LIBS)

dnl need 0.0.20 specifically.
PKG_CHECK_MODULES(GEGL, gegl >= $GEGL_VERSION)
dnl we want to use geglmm.
dnl 0.0.17.1 because of bugs in 0.0.17
PKG_CHECK_MODULES(GEGLMM, geglmm >= $GEGLMM_VERSION)
AC_SUBST(GEGLMM_CFLAGS)
AC_SUBST(GEGLMM_LIBS)

PKG_CHECK_MODULES(GOOCANVASMM, [goocanvasmm-1.0 >= $LIBGOOCANVASMM_VERSION])


if test $HAVE_GNOME_VFS_2_14 = 1
then
	NIEPCE_BUILD_CONFIG="$NIEPCE_BUILD_CONFIG Gnome-VFS>=2.14"
else
	NIEPCE_BUILD_CONFIG="$NIEPCE_BUILD_CONFIG Gnome-VFS=2.12"	
fi

AC_DEFINE_UNQUOTED([HAVE_GNOME_VFS_2_14], [$HAVE_GNOME_VFS_2_14], [If gnome-vfs is at least 2.14])

BOOST_REQUIRE([$BOOST_VERSION])
BOOST_BIND
BOOST_CONVERSION
BOOST_FILESYSTEM
BOOST_FORMAT
BOOST_FUNCTION
BOOST_SMART_PTR
BOOST_TEST([s])
BOOST_THREADS
BOOST_SIGNALS

AC_LANG_PUSH(C++)
if test "$GCC" = "yes"; then
        NIEPCE_BUILD_CONFIG="$NIEPCE_BUILD_CONFIG gcc-options="
        for option in -Wall -Wextra -Wsign-compare -Wpointer-arith \
                      -Wchar-subscripts -Wwrite-strings -Wmissing-noreturn \
                      -Wunused -Wpointer-arith -Wshadow -fshow-column ; do
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
ALL_LINGUAS="fr ru"
AM_GLIB_GNU_GETTEXT

NIEPCE_LOCALEDIR=[${datadir}/locale]
AC_SUBST(NIEPCE_LOCALEDIR)

AC_DEFINE_UNQUOTED([NIEPCE_BUILD_CONFIG], ["$NIEPCE_BUILD_CONFIG"], [The string used to hardcode the build config.])

SHAVE_INIT

AC_CONFIG_FILES([
Makefile
shave
shave-libtool
art/Makefile
doc/Doxyfile
doc/Makefile
doc/doxygen/Makefile
data/Makefile
data/icons/Makefile
data/themes/Makefile
src/Makefile
src/ext/Makefile
src/ext/libgdl/Makefile
src/niepce/ui/Makefile
src/niepce/ui/thumb-view/Makefile
src/niepce/modules/Makefile
src/niepce/modules/darkroom/Makefile
src/niepce/Makefile
src/fwk/Makefile
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
