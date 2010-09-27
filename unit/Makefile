
TESTS = $(wildcard *test.c)
TARGETS = $(patsubst %.c, %, $(TESTS))

CPPFLAGS =
CFLAGS = -O0 -g3 -pthread
CC = gcc
LL = gcc
CONFIGPATH = Debug_pthread
LDFLAGS = -L../libs/$(CONFIGPATH) -pthread
LDLIBS = -looc

debug : $(TARGETS)

clean :
	rm -f *.o
	rm -f $(TARGETS)

