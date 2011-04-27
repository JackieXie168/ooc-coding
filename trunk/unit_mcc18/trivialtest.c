
/* This is a OocTest class implementation file
 */

#include <string.h>

#include "../libs/testcase.h"

#include "foo.h"
#include "implement/foo.h"
#include "bar.h"
#include "implement/bar.h"
#include "barson.h"
#include "implement/barson.h"
#include "bardaughter.h"
#include "implement/bardaughter.h"

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
	
	chain_constructor( OocTest, self, params );
}

/* Destructor
 */

static
void
OocTest_destructor( OocTest self, OocTestVtable vtab )
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
static	void	BarBadGrandSon_destructor( BarBadGrandSon self, BarBadGrandSonVtable vtab ) {}
static	int		BarBadGrandSon_copy( BarBadGrandSon self, const BarBadGrandSon from ) { return OOC_COPY_DEFAULT; }


static
void
test_class_table( void )
{
	assertTrue( FooClass.size == sizeof(struct FooObject)/sizeof(char) );
	assertZero( strcmp( FooClass.name, (char*) "Foo" ) );
	assertTrue( FooClass.parent == & BaseClass );
	assertNotNull( FooClass.vtable );
	assertTrue( FooClass.vtab_size == sizeof(struct FooVtable_stru)/sizeof(char) );

	assertTrue( BarClass.size == sizeof(struct BarObject)/sizeof(char) );
	assertZero( strcmp( BarClass.name, (char*) "Bar" ) );
	assertTrue( BarClass.parent == & BaseClass );
	assertNotNull( BarClass.vtable );
	assertTrue( BarClass.vtab_size == sizeof(struct BarVtable_stru)/sizeof(char) );

	assertTrue( BarSonClass.size == sizeof(struct BarSonObject)/sizeof(char) );
	assertZero( strcmp( BarSonClass.name, (char*) "BarSon" ) );
	assertTrue( BarSonClass.parent == & BarClass );
	assertNotNull( BarSonClass.vtable );
	
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.size == sizeof(struct BarBadGrandSonObject)/sizeof(char) );
	assertZero( strcmp( BarBadGrandSonClass.name, (char*) "BarBadGrandSon" ) );
	assertTrue( BarBadGrandSonClass.parent == & BarSonClass );
	assertTrue( BarBadGrandSonClass.vtable == (Vtable) & BarBadGrandSonVtableInstance);
	assertTrue( BarBadGrandSonClass.vtab_size == sizeof(struct BarBadGrandSonVtable_stru)/sizeof(char) );
	assertTrue( BarBadGrandSonClass.init == BarBadGrandSon_initialize );
	assertTrue( BarBadGrandSonClass.finz == BarBadGrandSon_finalize );
	assertTrue( BarBadGrandSonClass.ctor == (void (*)( Object, const void *))	BarBadGrandSon_constructor );
	assertTrue( BarBadGrandSonClass.dtor == (void (*)( Object, Vtable))			BarBadGrandSon_destructor );
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

#ifndef OOC_NO_DYNAMIC_MEM
	/* Check if vtable has delete operator */
	assertNotNull( ((BaseVtable)( FooClass.vtable ))->_delete );
	assertNotNull( ((BaseVtable)( BarClass.vtable ))->_delete );
	assertNotNull( ((BaseVtable)( BarSonClass.vtable ))->_delete );
#endif
	
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
#ifndef OOC_NO_DYNAMIC_MEM
	BarBadGrandSon bbgs;
#else
	static struct BarBadGrandSonObject bbgso;
	BarBadGrandSon bbgs = & bbgso;
#endif
	
	ooc_init_class( BarBadGrandSon );
	
#ifndef OOC_NO_DYNAMIC_MEM
	bbgs = (BarBadGrandSon) ooc_new( BarBadGrandSon, NULL );
#else
	ooc_use( &bbgso, BarBadGrandSon, NULL );
#endif
	{
#ifndef OOC_NO_DYNAMIC_MEM
		ooc_manage_object( bbgs );
#else
		ooc_manage( bbgs , (ooc_destroyer) ooc_release);
#endif
	
		BarBadGrandSonVirtual( bbgs )->BarSon.barson_virtual( bbgs );
		BarBadGrandSonVirtual( bbgs )->BarSon.Bar.bar_virtual( bbgs );
		
		try {
			BarBadGrandSonVirtual( bbgs )->uninitialized_virtual();
			fail();
		}
		catch_any
			assertTrue( exception_get_error_code( exception ) == err_undefined_virtual );
		end_try;
		
#ifndef OOC_NO_DYNAMIC_MEM
		ooc_delete( ooc_pass( (Object) bbgs ) );
#else
		ooc_release( ooc_pass( (Object) bbgs ) );
#endif
	}
	
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

static	void	FooLife_destructor( FooLife self, FooLifeVtable vtab )
													{ foolife_last_called++; }
static	int	FooLife_copy( FooLife self, const FooLife from )
													{ foolife_last_called++;
													  return OOC_COPY_DEFAULT; }

void
test_lifecycle( void )
{
#ifndef OOC_NO_DYNAMIC_MEM

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

#endif
}

void
test_lifecycle_static( void )
{
	static struct FooLifeObject foolife1, foolife2;
	
		foolife_last_called = foolife_nothing;
	ooc_init_class( FooLife );
		assertTrue( foolife_last_called == foolife_init );
	ooc_use( &foolife1, FooLife, NULL );
		assertTrue( foolife_last_called == foolife_ctor );
	ooc_copy( &foolife2, (Object) &foolife1 );
		assertTrue( foolife_last_called == foolife_copy );
	ooc_release( (Object) &foolife1 );
		assertTrue( foolife_last_called == foolife_dtor );
	ooc_finalize_class( FooLife );
		assertTrue( foolife_last_called == foolife_finz );
	
	/* prevent leaking foolife2 */
	ooc_init_class( FooLife );
	ooc_release( (Object) &foolife2 );
	ooc_finalize_class( FooLife );
}

static
void
test_new( void )
{
#ifndef OOC_NO_DYNAMIC_MEM

	Foo foo;
	
	foo = foo_new();
	assertNull( foo->text );
	assertZero( foo->data );
	ooc_delete( (Object) foo );
	
	foo = foo_new_with_data( 100 );
	assertNull( foo->text );
	assertTrue( foo->data == 100 );
	ooc_delete( (Object) foo );

	foo = foo_new_with_const_text( "copyable test text" );
	assertZero( strcmp( foo->text, "copyable test text" ) );
	assertZero( foo->data );
	ooc_delete( (Object) foo );

#endif
}

static
void
test_use( void )
{
	static struct FooObject foo;
	
	foo_use( &foo );
	assertNull( foo.text );
	assertZero( foo.data );
	ooc_release( (Object) &foo );
	
	foo_use_with_data( &foo, 100 );
	assertNull( foo.text );
	assertTrue( foo.data == 100 );
	ooc_release( (Object) &foo );

	foo_use_with_text( &foo, (char*) "copyable test text" );
	assertZero( strcmp( foo.text, (char *) "copyable test text" ) );
	assertZero( foo.data );
	ooc_release( (Object) &foo );

	foo_use_with_data( &foo, 200 );
	assertNull( foo.text );
	assertTrue( foo.data == 200 );
	ooc_release( (Object) &foo );
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
	TEST(test_lifecycle_static),
	TEST(test_virtual_initialization),
	TEST(test_new),
	TEST(test_use),	
	
	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
void main( void )
{
	static struct OocTestObject ooctest;
	int result;
	
	ooc_init_class( OocTest );
	ooc_use( &ooctest, OocTest, &methods );
	result = testcase_run( &ooctest );
	ooc_release( (Object) &ooctest );
	ooc_finalize_all();

	printf( "%s test result = %d\n", __FILE__, result );

	abort();
}
