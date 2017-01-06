pkglib_LTLIBRARIES = libnptxt.la

ACLOCAL_AMFLAGS= -I m4

libnptxt_la_SOURCES=src/nptxt.c 
libnptxt_la_CFLAGS=${GOBJ_DEPS_CFLAGS}  ${GLIB_DEPS_CFLAGS}
libnptxt_la_LDFLAGS=-module -avoid-version -shared
libnptxt_la_LIBADD=${GOBJ_DEPS_LIBS} ${GLIB_DEPS_LIBS}
