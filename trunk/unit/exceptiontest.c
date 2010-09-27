
/* This is a ExceptionTest class implementation file
 */

#include "../libs/ooc.h"
#include "../libs/exception.h"
#include "../libs/testcase.h"

/** @class ExceptionTest
 *  @brief ExceptionTest class - brief description.
 * 	@see exceptiontest.h
 */

/** @file exceptiontest.h 
 *  @brief ExceptionTest class - brief description.
 * ExceptionTest is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

DeclareClass( ExceptionTest, TestCase );

Virtuals( ExceptionTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( ExceptionTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( ExceptionTest, TestCase );


/* Class initializing
 */

static void exceptiontest_before_class( ExceptionTest self );
static void exceptiontest_before( ExceptionTest self );
static void exceptiontest_after( ExceptionTest self );
static void exceptiontest_after_class( ExceptionTest self );

static
void
ExceptionTest_initialize( Class this )
{
	ExceptionTestVtable vtab = (ExceptionTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) exceptiontest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) exceptiontest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) exceptiontest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) exceptiontest_after_class;

	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
ExceptionTest_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
ExceptionTest_constructor( ExceptionTest self, const void * params )
{
	assert( ooc_isInitialized( ExceptionTest ) );
	
	chain_constructor( ExceptionTest, self, NULL );
	
	self->TestCase.methods = (const struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
ExceptionTest_destructor( ExceptionTest self )
{
}

/* Copy constuctor
 */

static
int
ExceptionTest_copy( ExceptionTest self, const ExceptionTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
exceptiontest_before_class( ExceptionTest self )
{
	return;
}

static
void
exceptiontest_before( ExceptionTest self )
{
	return;
}

static
void
exceptiontest_after( ExceptionTest self )
{
	return;
}

static
void
exceptiontest_after_class( ExceptionTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

void
static
exceptiontest_method1( ExceptionTest self )
{
	failMsg("Intentionally failed");
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
struct TestCaseMethod methods[] =
{
	
	TEST(exceptiontest_method1),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
int main(int argc, char * argv[])
{
	ExceptionTest exceptiontest;
	
	ooc_init_class( ExceptionTest );
	exceptiontest = (ExceptionTest) ooc_new( ExceptionTest, &methods );
	return testcase_run((TestCase)exceptiontest);
}
