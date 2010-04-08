AM_CPPFLAGS = -DNDEBUG

pkglib_LIBRARIES=libooc.a

libooc_a_SOURCES= ooc.c exception.c refcounted.c list.c vector.c signal.c 

nobase_pkginclude_HEADERS= *.h implement/*.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ooc.pc

EXTRA_DIST = libooc.vcproj
