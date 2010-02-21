#include "ooc.h"
#include "exception.h"

/* This is a class implementation file
 */

#include "vector.h"

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
	
	vector_item_destroyer	destroy;

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
	VectorIndex		size;
	vector_item_destroyer destroy;
};

static
void
Vector_constructor( Vector self, const void * params )
{
	struct vector_const_params const * p = params;
	
	assert( ooc_isInitialized( Vector ) );
	
	chain_constructor( Vector, self, params );
	
	self->allocation_chunks	= p->size;
	self->destroy = p->destroy;
	
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
	
	if( self->destroy )
		for( i = 0; i < self->number_of_items; i++ )
			self->destroy( self->items[ i ] );
		
	ooc_free_and_null( (void **) & self->items );
}

/* Copy constuctor
 */

static
int
Vector_copy( Vector self, const Vector from )
{
	ooc_throw( exception_new( err_can_not_be_duplicated ) );
	
	return TRUE;
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
	assert( ooc_isInstanceOf( self, Vector ) );
	
	vector_realloc_if_needed( self );
	
	self->items[ self->number_of_items ++ ] = data;

	return self->number_of_items - 1;
}

VectorIndex
vector_insert( Vector self, VectorIndex position, void * data )
{
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items < position )
		ooc_throw( exception_new( err_wrong_position ) );	
	
	vector_realloc_if_needed( self );
	
	if( position != self->number_of_items )
		memmove(& ( self->items[ position + 1 ] ), 								/* destination */
				& ( self->items[ position ] ),									/* source */
				( self->number_of_items - position ) * sizeof( VectorItem ) );	/* len */
	
	self->items[ position ] = data;
	
	self->number_of_items ++;
	
	return position;
}

void
vector_delete_item( Vector self, VectorIndex position )
{
	VectorItem tmp;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= position )
		ooc_throw( exception_new( err_wrong_position ) );	
	
	tmp = self->items[ position ];
	
	if( position != self->number_of_items-1 )
		memmove(& ( self->items[ position ] ), 											/* destination */
				& ( self->items[ position + 1 ] ),										/* source */
				( self->number_of_items - (position + 1) ) * sizeof( VectorItem ) );	/* len */
	
	self->number_of_items --;
	
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
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= index )
		ooc_throw( exception_new( err_wrong_position ) );
		
	self->items [ index ]	=	data;
}

void
vector_swap( Vector self, VectorIndex p1, VectorIndex p2 )
{
	VectorItem tmp;
	
	assert( ooc_isInstanceOf( self, Vector ) );
	
	if( self->number_of_items <= p1 || self->number_of_items <= p2 )
		ooc_throw( exception_new( err_wrong_position ) );
		
	tmp 				= self->items[ p1 ];
	self->items[ p1 ] 	= self->items[ p2 ];
	self->items[ p2 ]	= tmp;
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

