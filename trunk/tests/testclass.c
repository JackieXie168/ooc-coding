
/* This is an implementation file for test classes
 */

#include "implement/testclass.h"

#include <exception.h>

/* This is a TestBase class implementation
 */

/** @class TestBase
 *  @brief TestBase class - brief description.
 * 	@see testbase.h
 */

/** @file testbase.h 
 *  @brief TestBase class - brief description.
 * TestBase is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( TestBase, Base );

/* Class initializing
 */

static
void
TestBase_initialize( Class this )
{
}

/* Class finalizing
 */

static
void
TestBase_finalize( Class this )
{
}


/* Constructor
 */

static
void
TestBase_constructor( TestBase self, const void * params )
{
	assert( ooc_isInitialized( TestBase ) );
	
	chain_constructor( TestBase, self, NULL );
}

/* Destructor
 */

static
void
TestBase_destructor( TestBase self, TestBaseVtable vtab )
{
}

/* Copy constuctor
 */

static
int
TestBase_copy( TestBase self, const TestBase from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/* This is a TestInt class implementation
 */

/** @class TestInt
 *  @brief TestInt class - brief description.
 * 	@see testint.h
 */

/** @file testint.h 
 *  @brief TestInt class - brief description.
 * TestInt is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( TestInt, TestBase );

/* Class initializing
 */

static
void
TestInt_initialize( Class this )
{
}

/* Class finalizing
 */

static
void
TestInt_finalize( Class this )
{
}

/* Constructor
 */

static
void
TestInt_constructor( TestInt self, const void * params )
{
	assert( ooc_isInitialized( TestInt ) );
	
	chain_constructor( TestInt, self, NULL );
	
	self->data = *( (int *) params );
}

/* Destructor
 */

static
void
TestInt_destructor( TestInt self, TestIntVtable vtab )
{
}

/* Copy constuctor
 */

static
int
TestInt_copy( TestInt self, const TestInt from )
{
	return OOC_COPY_DEFAULT;
	}

/*	=====================================================
	Class member functions
 */


TestInt
testint_new( int param )
{
	ooc_init_class( TestInt );
	
	return (TestInt) ooc_new( TestInt, & param );
}

void
testint_set( TestInt self, int data )
{
	self->data = data;
}

int
testint_get( TestInt self )
{
	return self->data;
}


/* This is a TestStr class implementation
 */

/** @class TestStr
 *  @brief TestStr class - brief description.
 * 	@see teststr.h
 */

/** @file teststr.h 
 *  @brief TestStr class - brief description.
 * TestStr is a .....
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( TestStr, TestBase );

/* Class initializing
 */

static
void
TestStr_initialize( Class this )
{
}

/* Class finalizing
 */

static
void
TestStr_finalize( Class this )
{
}

/* Constructor
 */

static
void
TestStr_constructor( TestStr self, const void * params )
{
	assert( ooc_isInitialized( TestStr ) );
	
	chain_constructor( TestStr, self, NULL );
	
	self->data = ooc_strdup( params );
}

/* Destructor
 */

static
void
TestStr_destructor( TestStr self, TestStrVtable vtab )
{
	ooc_free_and_null( (void*) & self->data );
}

/* Copy constuctor
 */

static
int
TestStr_copy( TestStr self, const TestStr from )
{
	self->data = ooc_strdup( from->data );
	
	return OOC_COPY_DONE;
}

/*	=====================================================
	Class member functions
 */


TestStr
teststr_new( const char * param )
{
	ooc_init_class( TestStr );
	
	return (TestStr) ooc_new( TestStr, param );
}

void
teststr_set( TestStr self, const char * param )
{
	ooc_free( self->data );
	
	self->data = ooc_strdup( param );
}

const char *
teststr_get( TestStr self )
{
	return self->data;
}
