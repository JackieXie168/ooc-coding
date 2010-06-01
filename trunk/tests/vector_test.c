
#include "tests.h"

#include <ooc.h>
#include <exception.h>
#include <vector.h>

#include <stdio.h>

#include "testclass.h"

static
void
vector_test_pushback( void )
{
	Vector vector;
	int i;

	vector = vector_new_type( 10, TestInt, TRUE );
	
	#pragma omp parallel for private(i) schedule( guided, 10 )
	for( i = 0; i<100; i++ )
		vector_push_back( vector, testint_new( i ) );
		
	for( i = 0; i<100; i++ ) {
		TestInt actual = ooc_cast( vector_get_item( vector, i ), TestInt );
		printf( "TestInt object at %p has data of: %d\n", (void*) actual, testint_get( actual) );
		}
	
	ooc_delete( (Object) vector );
}

static
void
vector_test_type_safety_I( void )
{
	Vector volatile vector = NULL;

	vector = vector_new_type( 10, TestInt, TRUE );

	/* Must violate type safety
	 */
	
	try { 
		printf( "Generating type safety violation exception:\n" );
		vector_push_back( vector, teststr_new( "bad") );
		}
	catch_any
		printf( "\tOK - Type safety violation exception caught!\n" );
	end_try;
	
	ooc_delete( (Object) vector );
}

static
void
vector_test_type_safety_II( void )
{
	Vector volatile vector = NULL;
	int volatile 	caught = FALSE;

	vector = vector_new_type( 10, TestBase, TRUE );

	/* Must NOT violate type safety
	 */
	
	try { 
		printf( "Checking type safety violation exception:\n" );
		vector_push_back( vector, teststr_new( "ok") );
		vector_push_back( vector, testint_new( 3 ) );
		}
	catch_any {
		printf( "\tUnexpected type safety violation exception caught!\n" );
		caught = TRUE;
		}
	end_try;
	
	if( ! caught )
		printf( "\tOK.\n" );
		
	ooc_delete( (Object) vector );
}

void
vector_test( void )
{	
	ooc_init_class( Exception );
	ooc_init_class( Vector );
	
	vector_test_pushback();
	vector_test_type_safety_I();
	vector_test_type_safety_II();
	
	ooc_finalize_all( );
}


