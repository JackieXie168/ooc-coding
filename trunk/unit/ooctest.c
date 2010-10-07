
/* This is a OocTest class implementation file
 */

#include "../libs/testcase.h"

#include "implement/foo.h"
#include "implement/bar.h"
#include "implement/barson.h"

/** @class OocTest
 *  @brief OocTest class - brief description.
 * 	@see ooctest.h
 */

/** @file ooctest.h 
 *  @brief OocTest class - brief description.
 * OocTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./ooctest
 */ 

DeclareClass( OocTest, TestCase );

Virtuals( OocTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( OocTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( OocTest, TestCase );


/* Class initializing
 */

static void ooctest_before_class( OocTest self );
static void ooctest_before( OocTest self );
static void ooctest_after( OocTest self );
static void ooctest_after_class( OocTest self );

static
void
OocTest_initialize( Class this )
{
	OocTestVtable vtab = (OocTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) ooctest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) ooctest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) ooctest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) ooctest_after_class;

	ooc_init_class( Exception );
	
	/* Allocate global resources here */
}

/* Class finalizing
 */

static
void
OocTest_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
OocTest_constructor( OocTest self, const void * params )
{
	assert( ooc_isInitialized( OocTest ) );
	
	chain_constructor( OocTest, self, NULL );
	
	self->TestCase.methods = (const struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
OocTest_destructor( OocTest self )
{
}

/* Copy constuctor
 */

static
int
OocTest_copy( OocTest self, const OocTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
ooctest_before_class( OocTest self )
{
	return;
}

static
void
ooctest_before( OocTest self )
{
	return;
}

static
void
ooctest_after( OocTest self )
{
	return;
}

static
void
ooctest_after_class( OocTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

/* This is a BarBadGrandSon class implementation
 */

DeclareClass( BarBadGrandSon, BarSon );

ClassMembers( BarBadGrandSon, BarSon )
EndOfClassMembers;

Virtuals( BarBadGrandSon, BarSon )
	void (* uninitialized_virtual)( void );
EndOfVirtuals;

AllocateClass( BarBadGrandSon, BarSon );

static	void	BarBadGrandSon_initialize( Class this ) {}
static	void	BarBadGrandSon_finalize( Class this ) {}
static	void	BarBadGrandSon_constructor( BarBadGrandSon self, const void * params ) {}
static	void	BarBadGrandSon_destructor( BarBadGrandSon self ) {}
static	int		BarBadGrandSon_copy( BarBadGrandSon self, const BarBadGrandSon from ) { return OOC_COPY_DEFAULT; }


static
void
test_class_table( void )
{
	assertTrue( FooClass.size == sizeof(struct FooObject)/sizeof(char) );
	assertZero( strcmp( FooClass.name, "Foo" ) );
	assertTrue( FooClass.parent == & BaseClass );
	assertNotNull( FooClass.vtable );
	assertTrue( FooClass.vtab_size == sizeof(struct FooVtable_stru)/sizeof(char) );

	assertTrue( BarClass.size == sizeof(struct BarObject)/sizeof(char) );
	assertZero( strcmp( BarClass.name, "Bar" ) );
	assertTrue( BarClass.parent == & BaseClass );
	assertNotNull( BarClass.vtable );
	assertTrue( BarClass.vtab_size == sizeof(struct BarVtable_stru)/sizeof(char) );

	assertTrue( BarSonClass.size == sizeof(struct BarSonObject)/sizeof(char) );
	assertZero( strcmp( BarSonClass.name, "BarSon" ) );
	assertTrue( BarSonClass.parent == & BarClass );
	assertNotNull( BarSonClass.vtable );
	
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.size == sizeof(struct BarBadGrandSonObject)/sizeof(char) );
	assertZero( strcmp( BarBadGrandSonClass.name, "BarBadGrandSon" ) );
	assertTrue( BarBadGrandSonClass.parent == & BarSonClass );
	assertTrue( BarBadGrandSonClass.vtable == (Vtable) & BarBadGrandSonVtableInstance);
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.init == BarBadGrandSon_initialize );
	assertTrue( BarBadGrandSonClass.finz == BarBadGrandSon_finalize );
	assertTrue( BarBadGrandSonClass.ctor == (void (*)( Object, const void *))	BarBadGrandSon_constructor );
	assertTrue( BarBadGrandSonClass.dtor == (void (*)( Object))					BarBadGrandSon_destructor );
	assertTrue( BarBadGrandSonClass.copy == (int  (*)( Object, const Object))	BarBadGrandSon_copy );
}

static
void
test_init( void )
{
	assertTrue( ooc_isInitialized( OocTest ) );
	
	ooc_init_class( Foo );
	ooc_init_class( BarSon );

	assertTrue( ooc_isInitialized( Foo ) );
	assertTrue( ooc_isInitialized( Bar ) ); /* Parent must be initialized automatically */
	assertTrue( ooc_isInitialized( BarSon ) );
	
	/* Check if vtable has valid Class pointer */
	assertTrue( ((BaseVtable)( FooClass.vtable ))->_class == & FooClass );
	assertTrue( ((BaseVtable)( BarClass.vtable ))->_class == & BarClass );
	assertTrue( ((BaseVtable)( BarSonClass.vtable ))->_class == & BarSonClass );

	/* Check if vtable has delete operator */
	assertNotNull( ((BaseVtable)( FooClass.vtable ))->_delete );
	assertNotNull( ((BaseVtable)( BarClass.vtable ))->_delete );
	assertNotNull( ((BaseVtable)( BarSonClass.vtable ))->_delete );
	
	/* Ckeck if the vtable is inherited */
	assertTrue( ((BarVtable)( BarClass.vtable ))->bar_virtual ==
				((BarSonVtable)( BarSonClass.vtable ))->Bar.bar_virtual	);
				
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.vtable ))->_class_register_next == & BarClass );
	assertTrue( ((BaseVtable)( BarClass.vtable ))->_class_register_prev == & FooClass );
	assertTrue( ((BaseVtable)( BarClass.vtable ))->_class_register_next == & BarSonClass );
	assertTrue( ((BaseVtable)( BarSonClass.vtable ))->_class_register_prev == & BarClass );
	assertNull( ((BaseVtable)( BarSonClass.vtable ))->_class_register_next );
				
	ooc_finalize_class( BarSon );
	ooc_finalize_class( Bar );
	ooc_finalize_class( Foo );
	
	assertFalse( ooc_isInitialized( Foo ) );
	assertFalse( ooc_isInitialized( Bar ) );
	assertFalse( ooc_isInitialized( BarSon ) );
}

void
static
test_finalize( void )
{
	assertTrue( ooc_isInitialized( OocTest ) );
	
	ooc_init_class( Foo );
	ooc_init_class( BarSon );

	assertTrue( ooc_isInitialized( Foo ) );
	assertTrue( ooc_isInitialized( Bar ) ); /* Parent must be initialized automatically */
	assertTrue( ooc_isInitialized( BarSon ) );
	
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.vtable ))->_class_register_next == & BarClass );
	assertTrue( ((BaseVtable)( BarClass.vtable ))->_class_register_prev == & FooClass );
	assertTrue( ((BaseVtable)( BarClass.vtable ))->_class_register_next == & BarSonClass );
	assertTrue( ((BaseVtable)( BarSonClass.vtable ))->_class_register_prev == & BarClass );
	assertNull( ((BaseVtable)( BarSonClass.vtable ))->_class_register_next );
				
	ooc_finalize_class( Bar );
	
	/* Ckeck if class initialization chain is correct */
	assertNotNull( ((BaseVtable)( FooClass.vtable ))->_class_register_prev );
	assertTrue( ((BaseVtable)( FooClass.vtable ))->_class_register_next == & BarSonClass );
	assertNull( ((BaseVtable)( BarClass.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarClass.vtable ))->_class_register_next );
	assertTrue( ((BaseVtable)( BarSonClass.vtable ))->_class_register_prev == & FooClass );
	assertNull( ((BaseVtable)( BarSonClass.vtable ))->_class_register_next );
	
	ooc_finalize_class( BarSon );
	ooc_finalize_class( Foo );
	
	/* Ckeck if class initialization chain is correct */
	assertNull( ((BaseVtable)( FooClass.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( FooClass.vtable ))->_class_register_next );
	assertNull( ((BaseVtable)( BarClass.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarClass.vtable ))->_class_register_next );
	assertNull( ((BaseVtable)( BarSonClass.vtable ))->_class_register_prev );
	assertNull( ((BaseVtable)( BarSonClass.vtable ))->_class_register_next );

	assertFalse( ooc_isInitialized( Foo ) );
	assertFalse( ooc_isInitialized( Bar ) );
	assertFalse( ooc_isInitialized( BarSon ) );
}

static
void
test_virtual_initialization( void )
{
	BarBadGrandSon bbgs;
	
	ooc_init_class( BarBadGrandSon );
	
	bbgs = (BarBadGrandSon) ooc_new( BarBadGrandSon, NULL );
	
	BarBadGrandSonVirtual( bbgs )->BarSon.barson_virtual( (BarSon) bbgs );
	BarBadGrandSonVirtual( bbgs )->BarSon.Bar.bar_virtual( (Bar) bbgs );
	
	try {
		BarBadGrandSonVirtual( bbgs )->uninitialized_virtual();
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_undefined_virtual );
	end_try;
	
	ooc_delete( (Object) bbgs );
	
	ooc_finalize_class( BarBadGrandSon );
}

/* Testing class lifecycle
 */

DeclareClass( FooLife, Base );

ClassMembers( FooLife, Base )
EndOfClassMembers;

Virtuals( FooLife, Base )
EndOfVirtuals;

AllocateClass( FooLife, Base );

enum
foolife_status
{
	foolife_nothing,
	foolife_init,
	foolife_ctor,
	foolife_copy,
	foolife_dtor,
	foolife_finz
};

static enum foolife_status foolife_last_called;

static	void	FooLife_initialize( Class this ) 	{ foolife_last_called++; }
static	void	FooLife_finalize( Class this ) 		{ foolife_last_called++; }

static	void	FooLife_constructor( FooLife self, const void * params )
													{  foolife_last_called++; }

static	void	FooLife_destructor( FooLife self ) 	{ foolife_last_called++; }
static	int	FooLife_copy( FooLife self, const FooLife from )
													{ foolife_last_called++;
													  return OOC_COPY_DEFAULT; }

void
test_lifecycle( void )
{
	FooLife foolife1, foolife2;
	
		foolife_last_called = foolife_nothing;
	ooc_init_class( FooLife );
		assertTrue( foolife_last_called == foolife_init );
	foolife1 = (FooLife) ooc_new( FooLife, NULL );
		assertTrue( foolife_last_called == foolife_ctor );
	foolife2 = (FooLife) ooc_duplicate( (Object) foolife1 );
		assertTrue( foolife_last_called == foolife_copy );
	ooc_delete( (Object) foolife1 );
		assertTrue( foolife_last_called == foolife_dtor );
	ooc_finalize_class( FooLife );
		assertTrue( foolife_last_called == foolife_finz );
	
	/* prevent leaking foolife2 */
	ooc_init_class( FooLife );
	ooc_delete( (Object) foolife2 );
	ooc_finalize_class( FooLife );
}

/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
struct TestCaseMethod methods[] =
{
	
	TEST(test_class_table),
	TEST(test_init),
	TEST(test_finalize),
	TEST(test_lifecycle),
	TEST(test_virtual_initialization),
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
int main(int argc, char * argv[])
{
	OocTest ooctest;
	int result;
	
	ooc_init_class( OocTest );
	ooctest = (OocTest) ooc_new( OocTest, &methods );
	result = testcase_run((TestCase)ooctest);
	ooc_delete( (Object) ooctest );
	ooc_finalize_all();
	return result;
}


