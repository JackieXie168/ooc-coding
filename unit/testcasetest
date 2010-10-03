
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

DeclareClass( SignalTest, TestCase );

Virtuals( SignalTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( SignalTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( SignalTest, TestCase );


/* Class initializing
 */

static void signaltest_before_class( SignalTest self );
static void signaltest_before( SignalTest self );
static void signaltest_after( SignalTest self );
static void signaltest_after_class( SignalTest self );

static
void
SignalTest_initialize( Class this )
{
	SignalTestVtable vtab = (SignalTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) signaltest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) signaltest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) signaltest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) signaltest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
SignalTest_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
SignalTest_constructor( SignalTest self, const void * params )
{
	assert( ooc_isInitialized( SignalTest ) );
	
	chain_constructor( SignalTest, self, NULL );
	
	self->TestCase.methods = (const struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
SignalTest_destructor( SignalTest self )
{
}

/* Copy constuctor
 */

static
int
SignalTest_copy( SignalTest self, const SignalTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
signaltest_before_class( SignalTest self )
{
	return;
}

static
void
signaltest_before( SignalTest self )
{
	return;
}

static
void
signaltest_after( SignalTest self )
{
	return;
}

static
void
signaltest_after_class( SignalTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
signaltest_segfault( SignalTest self )
{
	int * bad_ptr = NULL;
	
	try {
		int i = *bad_ptr;
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, SegmentationFault ) );
	}
	end_try;
}

void
static
signaltest_fpfault( SignalTest self )
{
	int zero = 0;
	
	try {
		int i = 10/zero;
	}
	catch_any {
		assertTrue( ooc_isInstanceOf( exception, ArithmeticFault ) );
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
	
	TEST(signaltest_segfault),
	TEST(signaltest_fpfault),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
int main(int argc, char * argv[])
{
	SignalTest signaltest;
	int result;
	
	ooc_init_class( SignalTest );
	signaltest = (SignalTest) ooc_new( SignalTest, &methods );
	result = testcase_run((TestCase)signaltest);
	ooc_delete( (Object) signaltest );
	ooc_finalize_all();
	return result;
}
