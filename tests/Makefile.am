AM_LDFLAGS =
AM_CFLAGS = -fopenmp
AM_CPPFLAGS = -DNDEBUG -I../libs

noinst_PROGRAMS=test
test_LDADD=../libs/libooc.a
test_SOURCES=main.c tests.h testclass.c testclass.h vector_test.c signal_test.c signal_test.h manage_test.c

EXTRA_DIST = tests.vcproj

