#include "ooc.h"
#include "exception.h"

/* This is a class implementation file
 */

#include "vector.h"

/** @class Vector
 *  @brief Vector class - a standard ooc container.
 * 	@see vector.h
 */

/** @file vector.h
 * @brief Vector class - a standard ooc container.
 * Vector is a container class, that can hold pointers to any kind of data.
 * Every item stored in the list must work perfectly with the supplied list item destroyer!
 * In practice this means, that you can only store items in the vector that has the same 
 * deletion method.
 * There are two types of Vector: typed and non-typed vector. Typed Vector can hold items
 * of a given type or its subtype only, while non-typed Vector can hold any type of data,
 * that conforms to the supplied object destructor. 
 * When creating a Vector, a given amount of space, the chunk size (n*sizeof(void*)) is allocated.
 * If the Vector grows above this size, the Vector automatically reallocates the store, increasing
 * by the chunk size.
 * @note	Vector implementation is thread safe, in the manner, that adding to or deleting items from the Vector
 * 			will not mesh up the Vector. But the VectorIndices may become invalid if multiple threads modify the
 * 			same Vector object. As a consecvence the @c _foreach_ and @c _find_ methods may behave unexpectedly
 * 			if an other thread is modifying the Vector! Make your own locking if needed! 
 */

#include <string.h>

/* Private virtuals
 */
 
Virtuals( Vector, Base )

EndOfVirtuals;

/*   Private class members
 */
 
typedef void * VectorItem;
 
ClassMembers( Vector, Base )

	VectorIndex		allocated;
	VectorIndex		allocation_chunks;
	VectorItem *	items;
	VectorIndex		number_of_items;
	
	Class					type;
	vector_item_destroyer	destroy;
	
#ifndef NO_THREADS
	ooc_Mutex		modify;
#endif

EndOfClassMembers;

/* Class virtual function prototypes
 */

/* Allocating the class description table and the vtable
 */

AllocateClass( Vector, Base );

 
/************************************************
 * List class preparation
 */

static
void
Vector_initialize( Class this )
{
}

/* Class finalizing
 */

static
void
Vector_finalize( Class this )
{
}


/* Constructor
 */

struct vector_const_params
{
	VectorIndex				size;
	vector_item_destroyer	destroy;
	Class					type;
};

static
void
Vector_constructor( Vector self, const void * params )
{
	struct vector_const_params const * p = params;
	
	assert( ooc_isInitialized( Vector ) );
	
	chain_constructor( Vector, self, params );
	
	ooc_mutex_init( self->modify );
	
	self->allocation_chunks	= p->size;
	self->destroy = p->destroy;
	self->type = p->type;
	
	self->allocated = self->allocation_chunks;
	
	self->items = ooc_malloc( self->allocated * sizeof(VectorItem) );
}

/* Destructor
 */

static
void
Vector_destructor( Vector self )
{
	VectorIndex i;
	
	ooc_lock( self->modify );
	
	if( self->destroy )
		for( i = 0; i < self->number_of_items; i++ )
			self->destroy( self->items[ i ] );
		
	ooc_free_and_null( (void **) & self->items );
	
	ooc_mutex_release( self->modify );
}

/* Copy constuctor
 */

static
int
Vector_copy( Vector self, const Vector from )
{
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


Vector
vector_new( VectorIndex size, vector_item_destroyer destroy )
{
	struct vector_const_params p;
	
	p.size = size;
	p.destroy = destroy;
	p.type = NULL;
	
	return (Vector) ooc_new( Vector, & p );
}

Vector
_vector_new_type( VectorIndex size, Class type, int manage )
{
	struct vector_const_params p;
	
	if( ! _ooc_isClassOf( type, & BaseClass ) )
		ooc_throw( exception_new( err_bad_cast ) );
	
	p.size = size;
	p.type = type;
	if( manage )
		p.destroy = (vector_item_destroyer) ooc_delete;
	else
		p.destroy = NULL;
	
	return (Vector) ooc_new( Vector, & p );
}

Vector
vector_new_from_table( void * table, size_t record_size, VectorIndex num_of_records )
{
	Vector vector;
	char * tp;
	VectorIndex counter;
	struct vector_const_params p;
	
	p.size = num_of_records;
	p.destroy = NULL;
	p.type = NULL;
	
	vector = (Vector) ooc_new( Vector, & p );
	
	for( tp = table, counter = 0; counter < num_of_records; counter++, tp += record_size )
		vector->items[ counter ] = tp;
	
	vector->number_of_items = num_of_records;
		
	return vector;
}

static
void
vector_realloc_if_needed( Vector self )
{
	if( self->number_of_items == self->allocated ) {
		
		self->allocated += self->allocation_chunks;
		
		self->items = ooc_realloc( self->items, self->allocated * sizeof(VectorItem) );
		}
}

VectorIndex
vector_push_back( Vector self, void * data )
{
	ooc_manage( data, self->destroy );
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->type )
		ooc_check_cast( data, self->type );
	
	ooc_lock( self->modify );
	
	vector_realloc_if_needed( self );
	
	self->items[ self->number_of_items ++ ] = ooc_pass( data );
	
	ooc_unlock( self->modify );
	
	return self->number_of_items - 1;
}

VectorIndex
vector_insert( Vector self, VectorIndex position, void * data )
{
	ooc_manage( data, self->destroy );
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->type )
		ooc_check_cast( data, self->type );
	
	if( self->number_of_items < position )
		ooc_throw( exception_new( err_wrong_position ) );	
	
	ooc_lock( self->modify );
	
	vector_realloc_if_needed( self );
	
	if( position != self->number_of_items )
		memmove(& ( self->items[ position + 1 ] ), 								/* destination */
				& ( self->items[ position ] ),									/* source */
				( self->number_of_items - position ) * sizeof( VectorItem ) );	/* len */
	
	self->items[ position ] = ooc_pass( data );
	
	self->number_of_items ++;
	
	ooc_unlock( self->modify );
	
	return position;
}

void
vector_delete_item( Vector self, VectorIndex position )
{
	VectorItem tmp;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= position )
		ooc_throw( exception_new( err_wrong_position ) );	
	
	ooc_lock( self->modify );
	
	tmp = self->items[ position ];
	
	if( position != self->number_of_items-1 )
		memmove(& ( self->items[ position ] ), 											/* destination */
				& ( self->items[ position + 1 ] ),										/* source */
				( self->number_of_items - (position + 1) ) * sizeof( VectorItem ) );	/* len */
	
	self->number_of_items --;
	
	ooc_unlock( self->modify );
	
	if( self->destroy )
		self->destroy( tmp );
}

VectorIndex
vector_items( Vector self )
{
	assert( ooc_isInstanceOf( self, Vector ) );
	
	return self->number_of_items;
}

void *
vector_get_item( Vector self, VectorIndex index )
{
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= index )
		ooc_throw( exception_new( err_wrong_position ) );
		
	return self->items [ index ];
}

void
vector_set_item( Vector self, VectorIndex index, void * data )
{
	void * tmp;
	
	ooc_manage( data, self->destroy );
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= index )
		ooc_throw( exception_new( err_wrong_position ) );
		
	ooc_lock( self->modify );
	
	tmp = self->items [ index ];
	
	self->items [ index ]	=	ooc_pass( data );
	
	ooc_unlock( self->modify );
	
	if( self->destroy )
		self->destroy( tmp );
}

void
vector_swap( Vector self, VectorIndex p1, VectorIndex p2 )
{
	VectorItem tmp;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= p1 || self->number_of_items <= p2 )
		ooc_throw( exception_new( err_wrong_position ) );
		
	ooc_lock( self->modify );
	
	tmp 				= self->items[ p1 ];
	self->items[ p1 ] 	= self->items[ p2 ];
	self->items[ p2 ]	= tmp;

	ooc_unlock( self->modify );	
}

void
vector_foreach( Vector self, vector_item_executor execute, void * param )
{
	VectorIndex index;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	for( index = 0; index < self->number_of_items; index++ )
		execute( self->items[ index ], param );
}

VectorIndex
vector_foreach_until_true( Vector self, vector_item_checker match, void * param )
{
	VectorIndex index;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	for( index = 0; index < self->number_of_items; index++ )
		if( match( self->items[ index ], param ) )
			break;
	
	return index;
}

VectorIndex
vector_find_item( Vector self, VectorIndex from, vector_item_checker match, void * param )
{
	VectorIndex index;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= from )
		ooc_throw( exception_new( err_wrong_position ) );

	for( index = from; index < self->number_of_items; index++ )
		if( match( self->items[ index ], param ) )
			break;
	
	return index;
}

VectorIndex
vector_find_item_reverse( Vector self, VectorIndex from, vector_item_checker match, void * param )
{
	VectorIndex index;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= from )
		ooc_throw( exception_new( err_wrong_position ) );

	for( index = from + 1; index != 0; index-- )
		if( match( self->items[ index - 1 ], param ) )
			break;
	
	return index != 0 ? index - 1 : self->number_of_items;
}

