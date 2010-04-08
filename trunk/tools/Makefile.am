AM_LDFLAGS = `pkg-config --libs glib-2.0`
AM_CFLAGS = `pkg-config --cflags glib-2.0`
AM_CPPFLAGS = -DNDEBUG -DTEMPLATE_DIR='"$(pkgdatadir)/template"' 

bin_PROGRAMS=ooc
ooc_SOURCES=main.c
nobase_dist_pkgdata_DATA=template/*.c template/*.h template/implement/*.h

EXTRA_DIST = tools.vcproj gtkmm-vc90-2_4.vsprops gtkmm-vc90-d-2_4.vsprops
