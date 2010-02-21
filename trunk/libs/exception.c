

#include "ooc.h"

/* This is a class implementation file
 */

#include "implement/exception.h"

/* Allocating the class description table and the vtable
 */

AllocateClass( Exception, Base );

/* Class virtual function prototypes
 */


/* Class initializing
 */

static
void
Exception_initialize( Class this )
{
}

/* Class finalizing
 */

static
void
Exception_finalize( Class this )
{
}


/* Constructor
 */

static
void
Exception_constructor( Exception self, const void * params )
{
	chain_constructor( Exception, self, NULL );
	
	if( params )
        self->code = * ( (int*) params );
}

/* Destructor
 */

static
void
Exception_destructor( Exception self )
{
}

/* Copy constuctor
 */

static
int
Exception_copy( Exception self, const Exception from )
{
	/* makes the default object copying (bit-by-bit) */
	return FALSE;
}



/*	=====================================================
	Class member functions
 */


Object
exception_new( enum error_codes err_code )
{
	return ooc_new( Exception, &err_code );
}

int
exception_get_error_code( const Exception self )
{
	assert( ooc_isInstanceOf( self, Exception ));

	return self->code;
}

int
exception_get_subcode( const Exception self )
{
	assert( ooc_isInstanceOf( self, Exception ));

	return self->subcode;
}

