

#define COMPILING_OOC_C

#include "ooc.h"

#include "exception.h"

/** @file ooc.h
 * @brief Object Oriented C - macros and definitions.
 */
 
/**	Base Class.
 * Used for root Class in ooc. It must be a superclass for all classes.
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

static Class 		class_register = NULL;		/* Points to the most recently initialized Class */

#ifndef NO_THREADS
static ooc_Mutex	class_register_change;
#endif

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

		if( class_register == NULL )					/* Race condition, but however we declared _ooc_init_class() as non-thread-safe */
			ooc_mutex_init( class_register_change );
			
		ooc_lock( class_register_change );
		if( class_register ) 
			class_register->vtable->_class_register_next = self;
		self->vtable->_class_register_prev = class_register;
		self->vtable->_class_register_next = NULL;
		class_register = self;
		ooc_unlock( class_register_change );
	}
}

void
_ooc_finalize_class( const Class self )
{
	if( self->vtable->_class != NULL ) {

		self->vtable->_class = NULL;		/* Class is marked uninitalized */
		
		ooc_lock( class_register_change );

		if( class_register == self )		/* Move class register pointer if necessary */
			class_register = self->vtable->_class_register_prev;

		if( self->vtable->_class_register_prev )	/* Unchain the current class I. */
			self->vtable->_class_register_prev->vtable->_class_register_next = self->vtable->_class_register_next;
			
		if( self->vtable->_class_register_next )	/* Unchain the current class II. */
			self->vtable->_class_register_next->vtable->_class_register_prev = self->vtable->_class_register_prev;

		self->vtable->_class_register_prev = NULL;
		self->vtable->_class_register_next = NULL;
				
		if( class_register == NULL )
			ooc_mutex_release( class_register_change );
		else
			ooc_unlock( class_register_change );
			
		self->finz( self );					/* Finalize the current class */
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

STIN
void
ooc_build_object( Object object, const Class type, const void * params )
{
	/* Building the Object header */
	object->_vtab = type->vtable;		/* Vtable pointer */
	
		assert( sizeof( struct BaseObject ) == sizeof( struct BaseVtable * ) );
		/* If struct BaseObject has been changed, additional initialization might be missing here! */

	/* Constructs the object instance, that may fail */
	type->ctor( object, params );	
}

void
ooc_use_classptr( void * mem, const Class type, const void * params )
{
	Object object = (Object) mem;
	
	/* The class type must be initialized already! */
	assert( _ooc_isInitialized( type ) );
	
	memset( object, 0, type->size );  /* the constructor expects everything to be zeroed */
	
	ooc_build_object( object, type, params );
}
	
Object
ooc_new_classptr( const Class type, const void * params )
{
	Object object;

	/* The class type must be initialized already! */
	assert( _ooc_isInitialized( type ) );

	/* Allocates a memory block for the object instance, and initializes it with zeros */
	object = ooc_calloc(  1, type->size );

	{
	ooc_manage_object( object );

	ooc_build_object( object, type, params );

	return ooc_pass( object );
	}
}

/*	Copying an existing object
 */

static
void
copy_object_members( Object to, const Object from, const Class type )
{
	size_t offset, length;
							
	if( has_parent( type ) )
		copy_object_members( to, from, type->parent );
		
	switch( type->copy( to, from ) ) {
		
		case OOC_COPY_DONE:		break;
		
		case OOC_COPY_DEFAULT:	offset = has_parent( type ) ?  type->parent->size : sizeof( struct BaseObject );
								length = type->size - offset; 
					
								if( length )
									memcpy( ((char*)to)+offset, ((char*)from)+offset, length );
								break;
							
		case OOC_NO_COPY:
		default:				ooc_throw( exception_new( err_can_not_be_duplicated ) );
								break;
		};
}

Object
ooc_duplicate( const Object from )
{
	Object	duplicate;
	Class	type = from->_vtab->_class;

	/* Allocates memory for the object instance */
	duplicate = ooc_calloc( 1, type->size );
	{
	ooc_manage_object( duplicate );

	/* Initializes the Object header */
	duplicate->_vtab = from->_vtab;
		assert( sizeof( struct BaseObject ) == sizeof( struct BaseVtable * ) );
		/* If struct BaseObject has been changed, additional copying is missing here!
			memcpy( duplicate, from, sizeof(struct BaseObject)); would be a general solution, but much slover! */

	/* Calls the copy constructor */
	copy_object_members( duplicate, from, type );

	return ooc_pass( duplicate );
	}
}


/*	Deletes an object
 */

STIN
void
ooc_destroy_object( Object self )
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
}

void
ooc_release( Object self )
{
	ooc_destroy_object( self );
}

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
	ooc_destroy_object( self );
	
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
	
	do {
		if( actual->parent == base )
			return TRUE;
		actual = actual->parent;
		} while( actual );

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

int
_ooc_isClassOf( const Class this, const Class base )
{
	if( this == base )
		return TRUE;
	else
		return ooc_isClassChildOf( this, base );
}


void
ooc_check_cast( void * _self, const Class target )
{
	if( ! _ooc_isInstanceOf( _self, target ) )
		ooc_throw( exception_new( err_bad_cast ) );
		
	return;
}


Class
ooc_get_type( const Object self )
{
	if( (! self) || (! self->_vtab) || (! self->_vtab->_class) )
		ooc_throw( exception_new( err_bad_cast ) );

	return ( Class ) ( self->_vtab->_class );
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


/*	Helper: pointer read-out while nulling
 */

void *
ooc_ptr_read_and_null( void ** ptr_ptr )
{
	OOC_IMPLEMENT_PTR_READ_AND_NULL
}





