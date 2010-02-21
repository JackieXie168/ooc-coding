
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <assert.h>

#include "ooc.h"

#include "exception.h"

/*	Base Class address
 */

const struct ClassTable BaseClass;

#define has_parent(x) (x->parent != &BaseClass)

/*  Prototypes
 */
 
static void Base_delete( Object );

/* Class initialization
 */

static
void
invalid_virtual_function( void )
{
	 ooc_throw( exception_new( err_undefined_virtual ) ); 
}

/*	Calculating the offset of the first virtual function in the Vtable.
	Must be done this tricky, because the following assumption is not necessarily true:
	sizeof(struct BaseVtable) == offsetof( _dummyVtable, first_virtual_function)
	This assumption may fail, depending on the compiler and the system ! */

struct _dummyVtable {
	struct	BaseVtable_stru	Base;
	void	(* first_virtual_function)( void );
};
#define virtual_function_alignment (offsetof( struct _dummyVtable, first_virtual_function))


static
void
invalidate_vtable( const Class type )
{
	size_t i;
	void (**fnp)(void);

	if( type->vtab_size > virtual_function_alignment ) {

		i = ( type->vtab_size - virtual_function_alignment ) / sizeof( void(*)() );
		fnp = (void (**)(void)) ( ((char*) type->vtable) + virtual_function_alignment);

		for( ; i ; i--,  fnp++ )
			*fnp = invalid_virtual_function;
		}
}

static
void
inherit_vtable_from_parent( const Class self )
{
	if( has_parent(self) ) {

		assert( self->vtab_size >= self->parent->vtab_size );
		
		/* Inherit the overridden operators */
		self->vtable->_delete = self->parent->vtable->_delete;
		
		/* Inherit the virtual functions */
		if( self->parent->vtab_size > virtual_function_alignment )
		    memcpy( ((char*) self->vtable)+ virtual_function_alignment,			/* destination */
			    ((char*) self->parent->vtable) + virtual_function_alignment,	/* source */
			    self->parent->vtab_size - virtual_function_alignment );			/* bytes to copy */
		}
}

static Class class_register = NULL;

void
_ooc_init_class( const Class self )
{
	if( self->vtable->_class == NULL ) {

		if( has_parent( self ) )
			_ooc_init_class( self->parent );

		self->vtable->_class = self;
		self->vtable->_delete= Base_delete;

		invalidate_vtable( self );

		inherit_vtable_from_parent( self );

		self->init( self );

		self->vtable->_class_register_prev = class_register;
		self->vtable->_class_register_next = NULL;
		class_register = self;
	}
}

void
_ooc_finalize_class( const Class self )
{
	if( self->vtable->_class != NULL ) {

		self->vtable->_class = NULL;
		
		if( self->vtable->_class_register_prev )
			self->vtable->_class_register_prev->vtable->_class_register_next = self->vtable->_class_register_next;
		else
			class_register = self->vtable->_class_register_next;

		self->finz( self );			
	}	
}

void
ooc_finalize_all( void )
{
	while( class_register )
		_ooc_finalize_class( class_register );
}

/*	Creating a new Object
 */

Object
ooc_new_classptr( const Class type, const void * params )
{
	volatile Object object = NULL;

	/* The class type must be initialized already! */
	assert( _ooc_isInitialized( type ) );

	try {
		/* Allocates a memory block for the object instance, and initializes it with zeros */
		object = ooc_calloc(  1, type->size );

		/* Initializes the Object header */
		object->_vtab = type->vtable;		/* Vtable pointer */
			assert( sizeof( struct BaseObject ) == sizeof( struct BaseVtable * ) );
			/* If struct BaseObject has been changed, additional initialization is missing here! */

		/* Constructs the object instance, that may fail */
		type->ctor( object, params );	
		}
	catch_any {
		/* If the allocation or the constructor failed, frees the object */
		ooc_delete( object );
		ooc_rethrow();
		}
	end_try;

	return object;
}

/*	Copying an existing object
 */

static
void
copy_object_members( Object to, const Object from, const Class type )
{
	if( has_parent( type ) )
		copy_object_members( to, from, type->parent );

	if( ! type->copy( to, from ) ) {	/* If there is no copy constructor, we do a default copy */

			size_t offset, length;
			offset = has_parent( type ) ?  type->parent->size : sizeof( struct BaseObject );
			length = type->size - offset; 

			if( length )
				memcpy( ((char*)to)+offset, ((char*)from)+offset, length );
		}
}

Object
ooc_duplicate( const Object from )
{
	volatile Object	duplicate = NULL;
	Class	type = from->_vtab->_class;

	try {
		/* Allocates memory for the object instance */
		duplicate = ooc_calloc( 1, type->size );

		/* Initializes the Object header */
		duplicate->_vtab = from->_vtab;
			assert( sizeof( struct BaseObject ) == sizeof( struct BaseVtable * ) );
			/* If struct BaseObject has been changed, additional copying is missing here!
				memcpy( duplicate, from, sizeof(struct BaseObject)); would be a general solution, but much slover! */

		/* Calls the copy constructor */
		copy_object_members( duplicate, from, type );

		}
	catch_any {
		ooc_delete( duplicate );
		ooc_rethrow();
		}
	end_try;

	return duplicate;
}


/*	Deletes an object
 */

void
ooc_delete( Object self )
{
	if( self )
	   self->_vtab->_delete( self );
}

void
ooc_delete_and_null( Object * obj_ptr )
{
	assert( obj_ptr );

	/* Thread safe release of the object */
	ooc_delete( (Object) ooc_ptr_read_and_null( (void**) obj_ptr ) );
}

static
void
Base_delete( Object self )
{
	Class type;

	assert( self != NULL );
	
	type = self->_vtab->_class;

	/* destruct first */
	type->dtor( self );

	/* destruct the parents */
	while( has_parent( type ) ) {
		type = type->parent;
		type->dtor( self );
		}

	free( self );
}

/* Type checking helpers
 *
 ***********************************************************/

int
_ooc_isInitialized( const Class type )
{
	return type->vtable->_class == type ? TRUE : FALSE;
}

static
int
ooc_isClassChildOf( const Class checkable, const Class base )
{
	Class actual = checkable;
	
	while( has_parent(actual) ) {
		if( actual->parent == base )
			return TRUE;
		actual = actual->parent;
		}

	return FALSE;
}

int
_ooc_isInstanceOf( const void * _self, const Class base )
{
	Object self = (Object) _self;

	assert( _ooc_isInitialized( base ) );

	if( ! self )
		return FALSE;

	if( ! self->_vtab )
		return FALSE;

	if( ! self->_vtab->_class )
		return FALSE;

	if( self->_vtab->_class == base )
		return TRUE;

	if( self->_vtab->_class->vtable != self->_vtab )
		return FALSE;

	return ooc_isClassChildOf( self->_vtab->_class, base );
}

Class
ooc_get_type( const Object obj )
{
	return (Class ) ( obj->_vtab->_class );
}

/* Implementation of memory handling
 *
 ***********************************************************/

void *
ooc_malloc( size_t size )
{
	void * allocated;

	allocated = malloc( size );

	if( allocated == NULL )
		ooc_throw( exception_new( err_out_of_memory ) );

	return allocated;
}

void *
ooc_calloc( size_t num, size_t size )
{
	void * allocated;

	allocated = calloc( num, size );

	if( allocated == NULL )
		ooc_throw( exception_new( err_out_of_memory ) );

	return allocated;
}

void *
ooc_realloc( void *ptr, size_t size )
{
	void * allocated;

	allocated = realloc( ptr, size );

	if( allocated == NULL )
		ooc_throw( exception_new( err_out_of_memory ) );

	return allocated;
}

void *
ooc_memdup( const void * ptr, size_t size )
{
	void * duplicate = NULL;

	if( ptr ) {
		
		duplicate = ooc_malloc( size );
	
		memcpy( duplicate, ptr, size );

		}
		
	return duplicate;
}

char *
ooc_strdup( const char * s )
{
	return s ? (char *) ooc_memdup( s, strlen( s ) + 1 ) : NULL;	
}


void
ooc_free( const void * mem )
{
	if( mem )
		free( (void*) mem );
}

void
ooc_free_and_null( void ** mem )
{
	void * mem_ptr;

	assert( mem );

	/* Thread safe release of the memory while "nulling" the memory pointer */
	mem_ptr = ooc_ptr_read_and_null( mem );
	if( mem_ptr )
		free( mem_ptr );
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
	try_pt = block;
}


static
void
ooc_rewind_jmp_buf( void )
{
	if( try_pt ) {
		ooc_delete( try_pt->exc_obj );
		try_pt = try_pt->previous;
		}
	return;
};

void
ooc_throw( Object exc_obj_ptr )
{
	/* if we are in an exception handling, then it is a nesting, that is not allowed */
	if( try_pt )
		if( try_pt->exc_obj != NULL ) {
			ooc_rewind_jmp_buf();
			ooc_delete( exc_obj_ptr );
			exc_obj_ptr = NULL;		/* Will generate an err_bad_throw */
			}

	if( exc_obj_ptr == NULL )
		exc_obj_ptr = exception_new( err_bad_throw );

	if( try_pt ) {
		try_pt->exc_obj = exc_obj_ptr;
		longjmp( try_pt->buffer, !0 );
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

Object
ooc_exception_caught( const Class exc_class )
{
	Object ret_exc;

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

		if( try_pt->exc_obj != NULL ) {	/* If there was an exception */

			if ( !( try_pt->status & CAUGHT ) || ( try_pt->status & RETHROWN ) ) {
				/* if we have not caught the exception or has been rethrown, then rethrow it again */
				Object tmp = try_pt->exc_obj;
				try_pt = try_pt->previous;
				ooc_throw( tmp );
				}
			else
				/* if we have caught, and handled, just rewind the buffer stack */
				ooc_rewind_jmp_buf();

			}
		else /* If there was no exception, we simply unlink the try block pointer */
			try_pt = try_pt->previous;
	}
}


/*	Helper: pointer read-out while nulling
 */

void *
ooc_ptr_read_and_null( void ** ptr_ptr )
{
	
	#if defined( _MSC_VER ) && defined ( _M_IX86 )
	
		__asm {
			mov		EBX, ptr_ptr
			mov		EAX, 0
			xchg	[EBX], EAX
			}
	
	#elif defined( __GNUC__ ) && defined( __i386__ )
	
		void * ret_val;
		__asm__ ( 
			  "movl		%1, %%ebx;"
			  "movl		$0, %0;"
			  "xchgl	%0, (%%ebx)"
					: "=a" (ret_val)
					: "m" (ptr_ptr)
					: "ebx" );
		return ret_val;
	
	#else
	
		#warning "Implementation of ooc_ptr_read_and_null() is not thread safe."
		void * tmp = * ptr_ptr;
		* ptr_ptr = NULL;
		return tmp;
		
	#endif	
	
}





