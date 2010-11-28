
SOURCES = $(wildcard *.c)

TESTS = $(filter $(wildcard *test.c), $(SOURCES))
FAKES = $(filter-out $(TESTS), $(SOURCES))
FAKEOBJECTS = $(patsubst %.c, %.o, $(FAKES))
TARGETS = $(patsubst %.c, %, $(TESTS))

LDOBJECTS = $(FAKEOBJECTS)

CC = gcc
LL = gcc

# Automake targets

check : check-debug

check-debug : make_targets
check-debug : TESTABLE_TARGETS = ../libs/libooc_d.a
check-debug : CPPFLAGS =
check-debug : CFLAGS = -O0 -g3 -fopenmp
check-debug : LDFLAGS = -L../libs -fopenmp
check-debug : LDLIBS = -looc_d

check-release : make_targets
check-release : TESTABLE_TARGETS = ../libs/libooc.a
check-release : CPPFLAGS =
check-release : CFLAGS = -O2 -fopenmp
check-release : LDFLAGS = -L../libs -fopenmp
check-release : LDLIBS = -looc

# Eclipse targets

debug : make_targets
debug : TESTABLE_TARGETS = ../libs/Debug_gomp/libooc.a
debug : CPPFLAGS =
debug : CFLAGS = -O0 -g3 -fopenmp
debug : LDFLAGS = -L../libs/Debug_gomp -fopenmp
debug : LDLIBS = -looc

release : make_targets
release : TESTABLE_TARGETS = ../libs/Release_gomp/libooc.a
release : CPPFLAGS =
release : CFLAGS = -O2 -fopenmp
release : LDFLAGS = -L../libs/Release_gomp -fopenmp
release : LDLIBS = -looc

# Unused targets
all:
html:
pdf:
install:
install-html:
install-pdf:

# Building the targets

make_targets : $(TARGETS)

$(TARGETS) : $(FAKEOBJECTS) $(TESTABLE_TARGETS)

%.o : %.c
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $< -o $@

% : %.o
	$(LL) $(LDFLAGS) $< $(LDOBJECTS) $(LDLIBS) -o $@

clean :
	rm -f *.o
	rm -f $(TARGETS)

