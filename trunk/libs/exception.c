

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
 *		chain_constructor( MyException, self, NULL );
 * 
 *		if( params )
 *	        self->Exception.user_code = * ( (int*) params );
 * }
 * static void MyException_destructor( MyException self ) {}
 * static void MyException_copy( MyException self, MyException other ) { return OOC_COPY_DEFAULT; }
 * 
 * Exception myexception_new( int myerror_code ) 
 * { 
 * 		ooc_init_class( MyException );
 * 		return (Exception) ooc_new( MyException, & myerror_code );
 * }
 * 
 * ...
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
    else
    	self->code = err_user_code;
}

/* Destructor
 */

static
void
Exception_destructor( Exception self, ExceptionVtable vtab )
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


Exception
exception_new( enum error_codes err_code )
{
	return (Exception) ooc_new( Exception, &err_code );
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

/* Implementation of exception handling
 *
 ***********************************************************/

TLS struct ooc_try_block * try_pt = NULL;

/* status values */
#define	CAUGHT		001
#define RETHROWN	002


void
ooc_link_try_block( struct ooc_try_block * block )
{
	block->exc_obj	 = NULL;
	block->status    = 0;
	block->previous  = try_pt;
	block->managed	 = NULL;
	try_pt = block;
}


static void clear_managed_stack( void );

void
ooc_throw( Exception exc_obj_ptr )
{
	if( exc_obj_ptr == NULL )
		exc_obj_ptr = exception_new( err_bad_throw );

	/* Throwing Exception clears the managed stack */
	clear_managed_stack();
	 
	if( try_pt ) {
		
		/* if we are in an exception handling, then its a rethrow with the new Exception */
		if( try_pt->exc_obj != NULL ) {
			ooc_delete( (Object) try_pt->exc_obj );
			try_pt->exc_obj = exc_obj_ptr;
			try_pt->status |= RETHROWN;
			}
		else {
			try_pt->exc_obj = exc_obj_ptr;
			LONGJMP( try_pt->buffer, !0 );
			}
		}
	else {
		/* if no error handler was set, we call the system to handle the error */
		/*
		fprintf(stderr, "*** Unhandled exception! Exception object is:\n\n" );
		//dump_item( exc_obj_ptr, 128 );
		//dump_item( exc_obj_ptr->_vtab->_class, 16 );
		//dump_item( exc_obj_ptr->_vtab->_class->name, 16 );
		*/
		abort();
		}
}

Exception
ooc_exception_caught( const Class exc_class )
{
	Exception ret_exc;

	if( try_pt == NULL )
		ooc_throw( exception_new( err_bad_throw ) );
	
	if( try_pt->exc_obj == NULL )
		ooc_throw( exception_new( err_bad_throw ) );

	if( try_pt->status & CAUGHT )	/* Every exception can be caught only once */
		return NULL;

	if( exc_class )
		/* check if the actual exception is of the desired class */
		ret_exc = _ooc_isInstanceOf( try_pt->exc_obj, exc_class ) ? try_pt->exc_obj : NULL;
	else
		/* Parameter is NULL: catch anything, let's return the Exception pointer */
		ret_exc = try_pt->exc_obj;

	if( ret_exc )
		try_pt->status |= CAUGHT;

	return ret_exc;
}

void
ooc_rethrow( void )
{
	if( try_pt && (try_pt->status & CAUGHT) )
		try_pt->status |= RETHROWN;
	else
		ooc_throw( exception_new( err_bad_throw ) );
}

void
ooc_end_try( void )
{
	if( try_pt ) {
		
		assert( try_pt->managed == NULL );
		
		if( try_pt->exc_obj != NULL ) {	/* If there was an exception */

			if ( !( try_pt->status & CAUGHT ) || ( try_pt->status & RETHROWN ) ) {
				/* if we have not caught the exception or has been rethrown, then rethrow it again */
				Exception tmp = try_pt->exc_obj;
				try_pt = try_pt->previous;
				ooc_throw( tmp );
				}
			else {
				/* if we have caught, and handled, just rewind the buffer stack */
				ooc_delete( (Object) try_pt->exc_obj );
				try_pt = try_pt->previous;
				}

			}
		else /* If there was no exception, we simply unlink the try block pointer */
			try_pt = try_pt->previous;
	}
}

/* Managing objects
 */
 
TLS struct ooc_Manageable * managed = NULL;
 
void
ooc_chain_manageable( struct ooc_Manageable * manageable )
{
	if( try_pt ) {
		manageable->previous = try_pt->managed;
		try_pt->managed = manageable;
		}
	else {
		manageable->previous = managed;
		managed = manageable;
		}
}

void
ooc_unchain_last_manageable( void )
{	
	if( try_pt )
		try_pt->managed = try_pt->managed->previous;
	else
		managed = managed->previous;
}

void
ooc_unchain_manageable( void * target )
{
	if( try_pt ) {
		assert( try_pt->managed );
		assert( try_pt->managed->target == target );
		}
	else {
		assert( managed );
		assert( managed->target == target );
		}
	
	ooc_unchain_last_manageable();
}

static
void
clear_managed_stack( void )
{
	struct ooc_Manageable * stack;
	
	if( try_pt )
		stack = ooc_ptr_read_and_null( (void**) & try_pt->managed );
	else
		stack = ooc_ptr_read_and_null( (void**) & managed );
	
	while( stack ) {
		if( stack->destroyer && stack->target )
			stack->destroyer( stack->target );
		stack = stack->previous;
		}
}

/** @name Exception handling.
 * 
 * @{
 * 
 * @def try
 * Opens a try ... catch block.
 * It must include exactly one statement, or group of statements in curly braces.
 * @hideinitializer
 * 
 * @def catch( ec )
 * Catches an exception of a given class.
 * It must include exactly one statement, or group of statements in curly braces.
 * @param	ec		The name of the exception class to be caought. Must be a subclass of Exception.
 * @note	The predefined @c exception variable of Exception type can be used as the caught object.
 * You must not delete the @c exception Object!
 * @hideinitializer
 * 
 * @def catch_any
 * Catches any thrown Exceptions.
 * It must include exactly one statement, or group of statements in curly braces.
 * @note	The predefined @c exception variable of Exception type can be used as the caught object.
 * You must not delete the @c exception Object!
 * @hideinitializer
 * 
 * @def finally
 * Finally block.
 * This block is executed in every case: either exception was thrown or not, caught or not.
 * It must include exactly one statement, or group of statements in curly braces.
 * @hideinitializer
 * 
 * @def end_try
 * Closing mark for the try catch construct.
 * This must be used to terminate the try catch finally constructs.
 * @hideinitializer
 * 
 * @}
 */
 
