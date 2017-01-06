dnl Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([deadbeef-nptxt-plugin], [1.4.88], [ylis.fi], [DeaDBeeF NPTXT Plugin], [https://ylis.fi])
AC_CONFIG_SRCDIR([src/nptxt.c])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wall -Werror subdir-objects dist-xz no-dist-gzip])
AM_SILENT_RULES([yes])

dnl Override $PACKAGE to make $pkglibdir use deadbeef search path
PACKAGE=deadbeef

AC_PROG_CC_C99
AM_PROG_AR
AC_PROG_INSTALL
AC_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG

LT_INIT

PKG_CHECK_MODULES([GLIB_DEPS], [glib-2.0], , AC_MSG_ERROR([glibc-2 is required for this package]))
PKG_CHECK_MODULES([GOBJ_DEPS], [gobject-2.0], , AC_MSG_ERROR([gobject-2 is required for this package]))

AS_IF([test "x$ac_cv_prog_cc_c99" = "xno"], AC_MSG_ERROR([C99 Support is required]))

AC_CONFIG_FILES([Makefile])
AC_CONFIG_HEADERS(config.h)
AC_OUTPUT
