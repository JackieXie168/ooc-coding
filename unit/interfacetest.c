
/* This is a InterfaceTest class implementation file
 */

#include "../libs/testcase.h"

/** @class InterfaceTest
 *  @brief InterfaceTest class - brief description.
 * 	@see interfacetest.h
 */

/** @file interfacetest.h 
 *  @brief InterfaceTest class - brief description.
 * InterfaceTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./interfacetest
 */ 

DeclareInterface( MyInterface )

	void (* method1) ( Object );
	void (* method2) ( Object );

EndOfInterface;


DeclareClass( InterfaceTest, TestCase );

Virtuals( InterfaceTest, TestCase )

	Interface( MyInterface );

EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( InterfaceTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( InterfaceTest, TestCase );


/* Class initializing
 */

static void interfacetest_before_class( InterfaceTest self );
static void interfacetest_before( InterfaceTest self );
static void interfacetest_after( InterfaceTest self );
static void interfacetest_after_class( InterfaceTest self );

static void dummy_method( Object );

static
void
InterfaceTest_initialize( Class this )
{
	InterfaceTestVtable vtab = (InterfaceTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) interfacetest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) interfacetest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) interfacetest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) interfacetest_after_class;

	vtab->MyInterface.method1 				= 	dummy_method;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
InterfaceTest_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
InterfaceTest_constructor( InterfaceTest self, const void * params )
{
	assert( ooc_isInitialized( InterfaceTest ) );
	
	chain_constructor( InterfaceTest, self, params );
}

/* Destructor
 */

static
void
InterfaceTest_destructor( InterfaceTest self, InterfaceTestVtable vtab )
{
}

/* Copy constuctor
 */

static
int
InterfaceTest_copy( InterfaceTest self, const InterfaceTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
interfacetest_before_class( InterfaceTest self )
{
	return;
}

static
void
interfacetest_before( InterfaceTest self )
{
	return;
}

static
void
interfacetest_after( InterfaceTest self )
{
	return;
}

static
void
interfacetest_after_class( InterfaceTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

static
void
dummy_method( Object self )
{
	assertTrue( ooc_isInstanceOf( self, InterfaceTest ) );
	ooc_cast( self, InterfaceTest )->data++;
}

static
void
check_init( InterfaceTest self )
{
	self->data = 0;
	InterfaceTestVirtual( self )->MyInterface.method1( (Object) self );
	assertTrue( self->data == 1 );

	try {
		InterfaceTestVirtual( self )->MyInterface.method2( (Object) self );
		fail();
		}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_undefined_virtual );
	end_try;
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
static ROM_ALLOC struct TestCaseMethod methods[] =
{
	
	TEST(check_init),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
TESTCASE_MAIN
{
	InterfaceTest interfacetest;
	int result;
	
	ooc_init_class( InterfaceTest );
	interfacetest = ooc_new( InterfaceTest, &methods );
	result = testcase_run((TestCase)interfacetest);
	ooc_delete( (Object) interfacetest );
	ooc_finalize_all();
	return result;
}
