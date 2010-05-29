
SOURCE_FILES = ooc.c exception.c refcounted.c list.c vector.c signal.c

AM_CFLAGS=

pkglib_LIBRARIES = libooc.a libooc_d.a

libooc_a_CFLAGS = -O2 -g -DNDEBUG $(AM_CFLAGS)
libooc_a_SOURCES= $(SOURCE_FILES) 

libooc_d_a_CFLAGS = -O0 -g3  $(AM_CFLAGS)
libooc_d_a_SOURCES= $(SOURCE_FILES)

nobase_pkginclude_HEADERS= *.h implement/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ooc.pc

EXTRA_DIST = libooc.vcproj
