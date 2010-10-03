
/* This is a VectorTest class implementation file
 */

#include "../libs/testcase.h"

/* Testable */
#include "../libs/vector.h"

#include "foo.h"
#include "bar.h"
#include "barson.h"
#include "bardaughter.h"

/** @class VectorTest
 *  @brief VectorTest class - brief description.
 * 	@see vectortest.h
 */

/** @file vectortest.h 
 *  @brief VectorTest class - brief description.
 * VectorTest is a .....
 * @note	This class is a final class, can not be inherited.
 * @note	Run as: valgrind --leak-check=yes --quiet ./vectortest
 */ 

DeclareClass( VectorTest, TestCase );

Virtuals( VectorTest, TestCase )
EndOfVirtuals;

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( VectorTest, TestCase )

	int		data;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

AllocateClass( VectorTest, TestCase );


/* Class initializing
 */

static void vectortest_before_class( VectorTest self );
static void vectortest_before( VectorTest self );
static void vectortest_after( VectorTest self );
static void vectortest_after_class( VectorTest self );

static
void
VectorTest_initialize( Class this )
{
	VectorTestVtable vtab = (VectorTestVtable) this->vtable;
	
	((TestCaseVtable)vtab)->before_class	= 	(test_method_type) vectortest_before_class;
	((TestCaseVtable)vtab)->before			= 	(test_method_type) vectortest_before;
	((TestCaseVtable)vtab)->after			= 	(test_method_type) vectortest_after;
	((TestCaseVtable)vtab)->after_class		= 	(test_method_type) vectortest_after_class;

	ooc_init_class( Exception );
	ooc_init_class( Vector );
	/* Allocate global resources here */
}

/* Class finalizing#include "../libs/ooc.h"
#include "../libs/exception.h"
 * 
 */

static
void
VectorTest_finalize( Class this )
{
	/* Release global resources! */
}

/* Constructor
 */

static
void
VectorTest_constructor( VectorTest self, const void * params )
{
	assert( ooc_isInitialized( VectorTest ) );
	
	chain_constructor( VectorTest, self, NULL );
	
	self->TestCase.methods = (const struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
VectorTest_destructor( VectorTest self )
{
}

/* Copy constuctor
 */

static
int
VectorTest_copy( VectorTest self, const VectorTest from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*  =====================================================
 *  Test setup
 */
 
static
void
vectortest_before_class( VectorTest self )
{
	return;
}

static
void
vectortest_before( VectorTest self )
{
	return;
}

static
void
vectortest_after( VectorTest self )
{
	return;
}

static
void
vectortest_after_class( VectorTest self )
{
	return;
}


/*	=====================================================
	Test methods
 */

static
void
test_pushback( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	
	#pragma omp parallel for private(i) schedule( guided, 10 )
	for( i = 0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( i ) );
		
	assertTrue( vector_items( vector ) == 100 );
	
	ooc_delete( (Object) vector );
}

static
void
test_string_vector( void )
{
	Vector vector = vector_new( 10, (vector_item_destroyer) ooc_free );
	int i;

	#pragma omp parallel for private(i) schedule( guided, 10 )
	for( i = 0; i<100; i++ )
		vector_push_back( vector, ooc_strdup( "Test string." ) );
		
	assertTrue( vector_items( vector ) == 100 );
	
	ooc_delete( (Object) vector );
}

static
void
test_untyped( void )
{
	Vector vector = vector_new( 10, (vector_item_destroyer) ooc_delete );

	/* Must NOT violate type safety
	 */
	vector_push_back( vector, bar_new( ) );
	vector_push_back( vector, foo_new( ) );
	vector_push_back( vector, barson_new( ) );
	vector_push_back( vector, bardaughter_new( ) );
		
	ooc_delete( (Object) vector );
}

static
void
test_type_safety_I( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );

	/* Must violate type safety
	 */
	
	try { 
		vector_push_back( vector, bar_new_with_const_text( "bad") );
		fail();
		}
	catch_any {
		assertTrue( exception_get_error_code( exception ) == err_bad_cast );
	}
	end_try;
	
	ooc_delete( (Object) vector );
}

static
void
test_type_safety_II( void )
{
	Vector vector = vector_new_type( 10, Bar, TRUE );

	/* Must NOT violate type safety
	 */
	vector_push_back( vector, barson_new( ) );
	vector_push_back( vector, bardaughter_new( ) );
		
	ooc_delete( (Object) vector );
}

static
void
test_from_table( void )
{
	static
	struct TestData
	{
		int				counter;
		const char * 	text;
	}
	testData[] =
	{
		{ 0, "text 0" },
		{ 1, "text 1" },
		{ 2, "text 2" },
		{ 3, "text 3" },
		{ 4, "text 4" },
		{ 5, "text 5" },
		{ 6, "text 6" },
	};
	#define RECORDS (sizeof(testData)/sizeof(struct TestData))
	
	Vector vector = vector_new_from_table( &testData, sizeof(struct TestData), RECORDS );
	int i;
	
	assertTrue( vector_items( vector ) == RECORDS );
	for( i = 0; i<RECORDS; i++ )
		assertTrue( vector_get_item( vector, i ) == &testData[i] );

	ooc_delete( (Object) vector );
}

static
void
test_insert( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	
	for(i=0; i<100; i++ )
		if( i % 7 != 0 )
			vector_push_back( vector, foo_new_with_data( i ) );

	for(i=0; i<100; i++ )
		if( i % 7 == 0 )
			vector_insert( vector, i, foo_new_with_data( i ) );

	for(i=0; i<100; i++ ) {
		Foo actual = ooc_cast( vector_get_item( vector, i ), Foo );
		assertTrue( foo_get_data( actual ) == i );
	}
	
	ooc_delete( (Object) vector );	
}

static
void
test_delete( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i, j;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( i ) );
	assertTrue( vector_items( vector ) == 100 );

	for(i=0; i < vector_items( vector ); )
		if( i % 7 == 0 )
			vector_delete_item( vector, i );
		else
			i++;
	assertTrue( vector_items( vector ) < 100 );

	for(i=0, j=0; i < vector_items( vector ); i++, j++ ) {
		Foo actual;
		
		if( j % 7 == 0 )
			j++;
			
		actual = ooc_cast( vector_get_item( vector, i ), Foo );
		assertTrue( foo_get_data( actual ) == j );
		}
				
	ooc_delete( (Object) vector );
}

static
void
test_swap( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	
	for(i=0; i<50; i++ ) {
		vector_push_back( vector, foo_new_with_data( 2*i+1 ) );
		vector_push_back( vector, foo_new_with_data( 2*i ) );
		}
	assertTrue( vector_items( vector ) == 100 );

	for(i=0; i<50; i++ )
		vector_swap( vector, 2*i, 2*i+1 );
	assertTrue( vector_items( vector ) == 100 );
	
	for(i=0; i<100; i++ ) {
		Foo actual = ooc_cast( vector_get_item( vector, i ), Foo );
		assertTrue( foo_get_data( actual ) == i );
		}
	
	ooc_delete( (Object) vector );	
}

static const char foreach_param[] = "parameter";
static int foreach_counter;

static
void
test_item_executor( Foo item, void * param )
{
	static int previous;
	
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	assertTrue( foo_get_data( item ) == foreach_counter );
	foreach_counter++;
}

static
void
test_foreach( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( i ) );
	assertTrue( vector_items( vector ) == 100 );

	foreach_counter = 0;
	vector_foreach( vector, (vector_item_executor) test_item_executor, &foreach_param );
	assertTrue( foreach_counter == vector_items( vector ) );
	
	ooc_delete( (Object) vector );	
}

static
int
item_is_17( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	return ( foo_get_data( item ) == 17 ) ? TRUE : FALSE;
}

static
int
test_checker_always_false( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	return FALSE;
}

static
void
test_foreach_until( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	VectorIndex found;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );

	found = vector_foreach_until_true( vector, (vector_item_checker) test_checker_always_false, &foreach_param );
	assertTrue( found == vector_items( vector ) );
	
	found = vector_foreach_until_true( vector, (vector_item_checker) item_is_17, &foreach_param );
	assertTrue( found >= 0 && found < 100 );
	assertTrue( found != 17 ); /* This must be the index of the item, not the value of it! */
	assertTrue( foo_get_data( ooc_cast( vector_get_item( vector, found ), Foo ) ) == 17 );
	
	ooc_delete( (Object) vector );	
}

static
int
can_divide( int num )
{
	return ( num % 7 == 0 ) ? TRUE : FALSE;
}

static
int
test_checker_dividable( Foo item, void * param )
{
	assertTrue( ooc_isInstanceOf( item, Foo ) );
	assertTrue( param == foreach_param );
	
	foreach_counter++;
	
	return can_divide( foo_get_data( item ) );
}

static
void
test_find_item( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	VectorIndex found;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );

	found = vector_find_item( vector, 0, (vector_item_checker) test_checker_always_false, &foreach_param );
	assertTrue( found == vector_items( vector ) );
	
	foreach_counter = 0;
	for( found = 0; found < vector_items( vector ); found ++) {
		found = vector_find_item( vector, found, (vector_item_checker) test_checker_dividable, &foreach_param );
		if( found < vector_items( vector ) )
			assertTrue( can_divide( foo_get_data( ooc_cast( vector_get_item( vector, found ), Foo ) ) ) );
		}
	assertTrue( foreach_counter == 100 );
	
	ooc_delete( (Object) vector );	
}

static
void
test_wrong_position( void )
{
	Vector vector = vector_new_type( 10, Foo, TRUE );
	int i;
	
	try {
		vector_get_item( vector, 0 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;
	
	for(i=0; i<100; i++ )
		vector_push_back( vector, foo_new_with_data( 100 - i ) );
	assertTrue( vector_items( vector ) == 100 );
	
	try {
		vector_get_item( vector, -1 );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	try {
		vector_get_item( vector, vector_items( vector ) );
		fail();
	}
	catch_any
		assertTrue( exception_get_error_code( exception ) == err_wrong_position );
	end_try;

	ooc_delete( (Object) vector );	
}


/** Test methods order table.
 * Put your test methods in this table in the order they should be executed
 * using the TEST(method) macro. 
 * 
 */
 
struct TestCaseMethod methods[] =
{
	
	TEST(test_pushback),
	TEST(test_string_vector),
	TEST(test_untyped),
	TEST(test_type_safety_I),
	TEST(test_type_safety_II),
	TEST(test_from_table),
	TEST(test_insert),
	TEST(test_delete),
	TEST(test_swap),
	TEST(test_foreach),
	TEST(test_foreach_until),
	TEST(test_find_item),
	TEST(test_wrong_position),

	{NULL, NULL} /* Do NOT delete this line! */
};
	
/* Runs the test as an executable
 */
 
int main(int argc, char * argv[])
{
	VectorTest vectortest;
	int result;
	
	ooc_init_class( VectorTest );
	vectortest = (VectorTest) ooc_new( VectorTest, &methods );
	result = testcase_run((TestCase)vectortest);
	ooc_delete( (Object) vectortest );
	ooc_finalize_all();
	return result;
}
