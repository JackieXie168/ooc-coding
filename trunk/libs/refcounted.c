#include "ooc.h"

/* This is a class implementation file for the RefCounted class
 */

#include "implement/refcounted.h"

/** @class RefCounted
 *  @brief RefCounted class - a reference counted Object.
 * 	@see refcounted.h
 */

/** @file refcounted.h
 * @brief RefCounted class - a reference counted Object.
 * RefCounted serves as a base class for reference counted Objects.
 * @note The initial value for the counter is one.
 * @warning RefCounted overrides the delete operator: do not override the delete operator
 * for inherited classes!
 * @warning Current implementation of RefCounted is not thread safe.
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( RefCounted, Base );

/* The overridden delete operator
 */
 
static void (* original_delete)( Object );			/* Pointer to the original delete operator */
 
static
void
RefCounted_delete( RefCounted self )
{
	int to_be_deleted = FALSE;
	
	ooc_lock( self->access );
	if( self->counter != 0 )
		--self->counter;
	to_be_deleted = ( self->counter == 0 );
	ooc_unlock( self->access );
	
	if( to_be_deleted )
	   original_delete( (Object) self );
}


/* Class initializing
 */

static
void
RefCounted_initialize( Class this )
{
	/* Overriding the delete operator */
	
	original_delete       = this->vtable->_delete;
	this->vtable->_delete = (void (*)(Object)) RefCounted_delete;
}

/* Class finalizing
 */

static
void
RefCounted_finalize( Class this )
{
}

/* Constructor
 */

static
void
RefCounted_constructor( RefCounted self, const void * params )
{
	assert( ooc_isInitialized( RefCounted ) );
	
	chain_constructor( RefCounted, self, NULL );
	
    self->counter = 1;
    ooc_mutex_init( self->access );
}

/* Destructor
 */

static
void
RefCounted_destructor( RefCounted self )
{
    ooc_mutex_release( self->access );
}

/* Copy constuctor
 */

static
int
RefCounted_copy( RefCounted self, const RefCounted from )
{
    self->counter = 1;  				/* The new object has a new counter as well */
    ooc_mutex_init( self->access );
    
    return OOC_COPY_DONE;
}


/*	=====================================================
	Class member functions
 */

RefCounted
refcnt_addref( RefCounted self )
{
	assert( ooc_isInstanceOf( self, RefCounted ) );
	
	ooc_lock( self->access );
	self->counter ++;
	ooc_unlock( self->access );
	
	return self;
}


