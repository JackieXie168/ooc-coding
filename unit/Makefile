
TESTABLE_TARGETS = ../libs/Debug_gomp/libooc.a

SOURCES = $(wildcard *.c)

TESTS = $(filter $(wildcard *test.c), $(SOURCES))
FAKES = $(filter-out $(TESTS), $(SOURCES))
FAKEOBJECTS = $(patsubst %.c, %.o, $(FAKES))
TARGETS = $(patsubst %.c, %, $(TESTS))

CPPFLAGS =
CFLAGS = -O0 -g3 -fopenmp
CC = gcc
LL = gcc
CONFIGPATH = Debug_gomp
LDFLAGS = -L../libs/$(CONFIGPATH) -fopenmp
LDOBJECTS = $(FAKEOBJECTS)
LDLIBS =

debug : $(TARGETS)

$(TARGETS) : $(FAKEOBJECTS) $(TESTABLE_TARGETS)

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

% : %.o
	$(LL) $(LDFLAGS) $< $(LDOBJECTS) $(LDLIBS) -o $@

clean :
	rm -f *.o
	rm -f $(TARGETS)

