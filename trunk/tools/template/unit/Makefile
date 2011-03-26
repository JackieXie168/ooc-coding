# Makefile template for ooc unit test suits
#
# Requires GNU make varsion 3.81
#
# Your Testfiles must be located in the same directory.
# The test files MUST end with *test.c, this allows 
# the Makefile to identify the test files and create 
# the appropriate targets.
# The fake files must not and with *.test.c!
# Because of the linkind order, the symbols defined
# in the fake files will override the same symbols in
# the testable targets!
#
###############################################

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
check-debug : TESTABLE_TARGETS = 			# Set the testable targets here! Preferably a sinlge static library.
check-debug : CPPFLAGS =					# Preprocessor flags
check-debug : CFLAGS = -O0 -g3 -fopenmp		# Compiler flags for the test files
check-debug : LDFLAGS = -L???? -fopenmp		# Set the path for the testable targets and othe linkable libs
check-debug : LDLIBS = -l????				# set the library name for the testable targets and other linkable libs

check-release : make_targets
check-release : TESTABLE_TARGETS =
check-release : CPPFLAGS =
check-release : CFLAGS = -O2 -fopenmp
check-release : LDFLAGS = -L??? -fopenmp
check-release : LDLIBS = -l???

# Eclipse targets

debug : make_targets
debug : TESTABLE_TARGETS =
debug : CPPFLAGS =
debug : CFLAGS = -O0 -g3 -fopenmp
debug : LDFLAGS = -L??? -fopenmp
debug : LDLIBS = -l???

release : make_targets
release : TESTABLE_TARGETS =
release : CPPFLAGS =
release : CFLAGS = -O2 -fopenmp
release : LDFLAGS = -L??? -fopenmp
release : LDLIBS = -l???

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

