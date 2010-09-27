
/* This is a ProbaTest class implementation file
 */

/*
#include <ooc/ooc.h>
#include <ooc/exception.h>
#include <ooc/testcase.h>
*/

#include "../libs/ooc.h"
#include "../libs/exception.h"
#include "../libs/testcase.h"

/** @class ProbaTest
 *  @brief ProbaTest class - brief description.
 * 	@see probatest.h
 */

/** @file probatest.h 
 *  @brief ProbaTest class - brief description.
 * ProbaTest is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

DeclareClass( ProbaTest, TestCase );

Virtuals( ProbaTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( ProbaTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( ProbaTest, TestCase );


/* Class initializing
 */

static void probatest_before_class( ProbaTest self );
static void probatest_before( ProbaTest self );
static void probatest_after( ProbaTest self );
static void probatest_after_class( ProbaTest self );

static
void
ProbaTest_initialize( Class this )
{
	ProbaTestVtable vtab = (ProbaTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) probatest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) probatest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) probatest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) probatest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
ProbaTest_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
ProbaTest_constructor( ProbaTest self, const void * params )
{
	assert( ooc_isInitialized( ProbaTest ) );
	
	chain_constructor( ProbaTest, self, NULL );
	
	self->TestCase.methods = (const struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
ProbaTest_destructor( ProbaTest self )
{
}

/* Copy constuctor
 */

static
int
ProbaTest_copy( ProbaTest self, const ProbaTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
probatest_before_class( ProbaTest self )
{
	return;
}

static
void
probatest_before( ProbaTest self )
{
	return;
}

static
void
probatest_after( ProbaTest self )
{
	return;
}

static
void
probatest_after_class( ProbaTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
probatest_segfault( ProbaTest self )
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
probatest_fpfault( ProbaTest self )
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
	
	TEST(probatest_segfault),
	TEST(probatest_fpfault),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
int main(int argc, char * argv[])
{
	ProbaTest probatest;
	
	ooc_init_class( ProbaTest );
	probatest = (ProbaTest) ooc_new( ProbaTest, &methods );
	return testcase_run((TestCase)probatest);
}
