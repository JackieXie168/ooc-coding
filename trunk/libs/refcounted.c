#include "ooc.h"

/* This is a class implementation file for the RefCounted class
 */

#include "implement/refcounted.h"

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
	#ifndef _MSC_VER
		#warning "Implementation of RefCounted_delete() is not thread safe"
	#endif

	if( --( self->counter ) == 0 )
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
}

/* Destructor
 */

static
void
RefCounted_destructor( RefCounted self )
{
}

/* Copy constuctor
 */

static
int
RefCounted_copy( RefCounted self, const RefCounted from )
{
    self->counter = 1;  /* The new object has a new counter as well */
    return OOC_COPY_DONE;
}


/*	=====================================================
	Class member functions
 */

RefCounted
refcnt_addref( RefCounted self )
{
	assert( ooc_isInstanceOf( self, RefCounted ) );
	
	#ifndef _MSC_VER
		#warning "Implementation of refcnt_addref() is not thread safe"
	#endif

	self->counter ++;
	
	return self;
}


