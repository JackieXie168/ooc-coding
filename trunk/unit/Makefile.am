testable_library = $(top_srcdir)/libs/libooc.a

check_PROGRAMS = testcasetest ooctest exceptiontest refcountedtest listtest vectortest signaltest

check_LIBRARIES = libfakes.a
libfakes_a_SOURCES = bar.c barson.c bardaughter.c foo.c fooexception.c

test_suites = suite.sh

LDADD = ./libfakes.a $(testable_library)

TESTS = $(check_PROGRAMS) $(test_suites)

nobase_pkginclude_HEADERS= *.h implement/*.h

EXTRA_DIST = $(test_suites) *.bat Makefile.Windows MakefileEclipse.linux *.mcp

