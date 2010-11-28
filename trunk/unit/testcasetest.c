
/* This is a ProbaTest class implementation file
 */

#include "../libs/testcase.h"

/** @class SignalTest
 *  @brief SignalTest class - brief description.
 * 	@see probatest.h
 */

/** @file probatest.h 
 *  @brief SignalTest class - brief description.
 * SignalTest is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

DeclareClass( TestCaseTest, TestCase );

Virtuals( TestCaseTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( TestCaseTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( TestCaseTest, TestCase );


/* Class initializing
 */

static void testcasetest_before_class( TestCaseTest self );
static void testcasetest_before( TestCaseTest self );
static void testcasetest_after( TestCaseTest self );
static void testcasetest_after_class( TestCaseTest self );

static
void
TestCaseTest_initialize( Class this )
{
	TestCaseTestVtable vtab = (TestCaseTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) testcasetest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) testcasetest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) testcasetest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) testcasetest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
TestCaseTest_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
TestCaseTest_constructor( TestCaseTest self, const void * params )
{
	assert( ooc_isInitialized( TestCaseTest ) );
	
	chain_constructor( TestCaseTest, self, NULL );
	
	self->TestCase.methods = (const struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
TestCaseTest_destructor( TestCaseTest self, TestCaseTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
TestCaseTest_copy( TestCaseTest self, const TestCaseTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
testcasetest_before_class( TestCaseTest self )
{
	return;
}

static
void
testcasetest_before( TestCaseTest self )
{
	return;
}

static
void
testcasetest_after( TestCaseTest self )
{
	return;
}

static
void
testcasetest_after_class( TestCaseTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
segfault( TestCaseTest self )
{
	/* faking optimizing compilers: bad_ptr is volatile! */
	volatile int * bad_ptr = NULL;

	try {
		int i = *bad_ptr;
		failMsg( "Can fail in Valgrind!" );
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, SegmentationFault ) );
	}
	end_try;
}

void
static
fpfault( TestCaseTest self )
{
	/* faking optmizing compilers: zero is volatile! */
	volatile int zero = 0;

	try {
		zero = zero / zero;
		fail();
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, ArithmeticFault ) );
	}
	end_try;	
}

void
static
callnull( TestCaseTest self )
{
	void (* bad_ptr)(void) = NULL;
	
	try {
		bad_ptr(); /* may result a real segmentation fault in some cases :-( I don't understand ... :-( */
		fail();
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, SegmentationFault ) );
	}
	end_try;
}


/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
struct TestCaseMethod methods[] =
{
	
	TEST(segfault),
	TEST(fpfault),
	TEST(callnull),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
int main(int argc, char * argv[])
{
	TestCaseTest testcasetest;
	int result;
	
	ooc_init_class( TestCaseTest );
	testcasetest = (TestCaseTest) ooc_new( TestCaseTest, &methods );
	result = testcase_run((TestCase)testcasetest);
	ooc_delete( (Object) testcasetest );
	ooc_finalize_all();
	return result;
}
