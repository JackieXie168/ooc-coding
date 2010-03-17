

#include "ooc.h"

/* This is a class implementation file
 */

#include "implement/exception.h"

/** @class Exception
 *  @brief Exception class - a standard ooc error reporting class.
 * 	@see exception.h
 */

/** @file exception.h
 *  @brief Exception class - a standard ooc error reporting class.
 * Exception class is used to throw exceptions either by the ooc core, 
 * or by the user of the ooc as well.
 * Exception should be used as a base class for the User defined exception classes. 
 * Exceptions can be caught by class types, so if you would like
 * to distinguish the caught exceptions, always use a derived class!
 * The usual way of throwing an excetption is:
 * @code
 * ooc_throw( exception_new( err_out_of_memory ) );
 * @endcode
 * The recommended way of using your own exception class is:
 * @code
 * DeclareClass( MyException, Exception );
 * 
 * Virtuals( MyException, Exception )
 * EndOfVirtuals;
 * 
 * ClassMembers( MyException, Exception )
 * EndOfClassMembers;
 * 
 * AllocateClass( MyException, Exception );
 * 
 * static void MyException_initialize( Class this ) {}
 * static void MyException_finalize( Class this ) {}
 * static void MyException_constructor( MyException self, const void * params )
 * {
 *		self->code = err_user_code;
 *		if( params )
 *	        self->user_code = * ( (int*) params );
 * }
 * static void MyException_destructor( MyException self ) {}
 * static void MyException_copy( MyException self, MyException other ) { return OOC_COPY_DEFAULT; }
 * 
 * Object myexception_new( int myerror_code ) 
 * { 
 * 		ooc_init_class( MyException );
 * 		return ooc_new( MyException, & myerror_code );
 * }
 * 
 * try {
 * 		ooc_throw( myexception_new( 5 ) );
 * 	   }
 * catch( MyException ) {
 * 		printf( "MyException, code %d\n", exception_get_user_code( exception ) );
 *     }
 * catch( Exception ) {
 * 		printf( "ooc core Exception, code %d\n", exception_get_error_code( exception ) );
 *     }
 * end_try;
 * 
 * @endcode 
 *
 */
 
 /** @file implement/exception.h
  */
 
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
	return OOC_COPY_DEFAULT;
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
exception_get_user_code( const Exception self )
{
	assert( ooc_isInstanceOf( self, Exception ));

	return self->user_code;
}

