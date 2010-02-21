
#include "ooc.h"
#include "exception.h"

/* This is a class implementation file
 */

#include "implement/list.h"

/** @class List
 *  @brief List class - a standard ooc container.
 * 	@see list.h
 */

/** @file list.h
 * @brief List class - a standard ooc container.
 * List is a container class, that can hold pointers to any kind of data.
 * Every item stored in the list must work perfectly with the supplied list item destroyer!
 * In practice this means, that you can only store items in the list that has the same deletion method.
 * @warning	List implementation is not thread safe!
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( List, Base );

/* Class virtual function prototypes
 */

/*  Defining ListNode struct
 */

struct ListNode
{

    struct ListNode *		previous;
    struct ListNode *		next;
    
    void *					item;
};
 
/************************************************
 * List class preparation
 */

static
void
List_initialize( Class this )
{
}

/* Class finalizing
 */

static
void
List_finalize( Class this )
{
}


/* Constructor
 */

static
void
List_constructor( List self, const void * params )
{
	assert( ooc_isInitialized( List ) );
	
	chain_constructor( List, self, NULL );
	
	self->destroy = (list_item_destroyer) params;
}

/* Destructor
 */

static
void
List_destructor( List self )
{
	ListIterator p, next;
	
	next = self->first;
	
	self->first = self->last = NULL;
	
	while( next ) {
		p = next;
		next = next->next;
		
		if( self->destroy )
			self->destroy( p->item );

		ooc_free( p );
		}	
}

/* Copy constuctor
 */

static
int
List_copy( List self, const List from )
{
	ooc_throw( exception_new( err_can_not_be_duplicated ) );
	
	return TRUE;
}

/*	=====================================================
	List chain / unchain helpers
 */

static
ListIterator
create_new_item( const void * new_item )
{
	ListIterator tmp;

	tmp = (ListIterator) ooc_malloc( sizeof( struct ListNode )/sizeof(char) );
	
	tmp->item = (void * ) new_item;

	return tmp;	
}

static
ListIterator
chain_first_item( List self, ListIterator new_item )
{
	new_item->previous = new_item->next = NULL;
	
	self->first = self->last = new_item;

	return new_item;
}

static
ListIterator
chain_before( List self, ListIterator location, ListIterator new_item )
{
	new_item->previous 	= location->previous;
	new_item->next		= location;
	
	if( location->previous )
		location->previous->next = new_item;
		
	location->previous	= new_item;
	
	if( location == self->first )
		self->first = new_item;
	
	return new_item;	
}

static
ListIterator
chain_after( List self, ListIterator location, ListIterator new_item )
{
	new_item->previous 	= location;
	new_item->next		= location->next;
	
	if( location->next )
		location->next->previous = new_item;
		
	location->next 		= new_item;
	
	if( location == self->last )
		self->last = new_item;
	
	return new_item;
}

static
ListIterator
unchain( List self, ListIterator location )
{
	if( location->previous )
		location->previous->next = location->next;
		
	if( location->next )
		location->next->previous = location->previous;
		
	if( location == self->first )
		self->first = location->next;
		
	if( location == self->last )
		self->last = location->previous;
		
	return location;
}

/*	=====================================================
	Class member functions
 */

List
list_new( list_item_destroyer destroyer )
{
	assert( destroyer != NULL );
	
	return (List) ooc_new( List, destroyer );
}

ListIterator
list_append( List self, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	 
	if( self->last == NULL )
	
		return chain_first_item( self, create_new_item( new_item ) );
		
	else
	
		return chain_after( self, self->last, create_new_item( new_item ) );
		
}

ListIterator
list_prepend( List self, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	 
	if( self->first == NULL )
	
		return chain_first_item( self, create_new_item( new_item ) );
		
	else
	
		return chain_before( self, self->first, create_new_item( new_item ) );	
}

ListIterator
list_insert_before( List self, ListIterator location, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	 
	if( location == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
	 
	if( self->first == NULL )
	
		return chain_first_item( self, create_new_item( new_item ) );
		
	else
	
		return chain_before( self, location, create_new_item( new_item ) );
}

ListIterator
list_insert_after( List self, ListIterator location, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	
	if( location == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
	 
	if( self->first == NULL )
	
		return chain_first_item( self, create_new_item( new_item ) );
		
	else
	
		return chain_after( self, location, create_new_item( new_item ) );
}

void *
list_remove_item( List self, ListIterator location )
{
	ListIterator	removed;
	void *			item;

	assert( ooc_isInstanceOf( self, List ) );
	
	if( location == NULL || self->first == NULL )
		ooc_throw( exception_new( err_wrong_position ) );

	removed = unchain( self, location );
	item = removed->item;
	ooc_free( removed );

	return item;
}

void
list_delete_item( List self, ListIterator location )
{
	void *	item;

	assert( ooc_isInstanceOf( self, List ) );
	
	item = list_remove_item( self, location );

	if( self->destroy )
		self->destroy( item );	
}


ListIterator
list_first( List self)
{
	assert( ooc_isInstanceOf( self, List ) );
	
	return( self->first );
}

ListIterator
list_last( List self )
{
	assert( ooc_isInstanceOf( self, List ) );
	
	return( self->last );
}	

void *
list_get_item( ListIterator node )
{
	if( node  == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
		
	return node->item;
}

ListIterator
list_next( ListIterator node )
{
	if( node  == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
		
	return node->next;
}

ListIterator
list_previous( ListIterator node )
{
	if( node  == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
		
	return node->previous;
}

void
list_swap( ListIterator node1, ListIterator node2 )
{
	void * tmp;
	
	if( node1 == NULL || node2 == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
	
	tmp = node1->item;
	node1->item = node2->item;
	node2->item = tmp;
}

void
list_foreach( List self, list_item_executor func, void * param )
{
	ListIterator p;
	
	assert( ooc_isInstanceOf( self, List ) );

	for( p = self->first; p; p = p->next )
		func( p->item, param ); 
}

ListIterator
list_foreach_until_true( List self, ListIterator from, list_item_checker func, void * param )
{
	ListIterator p;
	
	assert( ooc_isInstanceOf( self, List ) );

	for( p = from ? from : self->first; p; p = p->next )
		if( func( p->item, param ) )
			break;
			 
	return p;
}

void
list_foreach_delete_if( List self, list_item_checker func, void * param )
{
	ListIterator act, next;
	
	assert( ooc_isInstanceOf( self, List ) );

	next = list_first( self );
	
	while ( ( act = next ) ) {

		next = list_next( act );

		if( func( act, param ) )
			list_delete_item( self, act );
		}
}


ListIterator
list_find_item( ListIterator node, list_item_checker fn_match, void * param )
{
	ListIterator p;
	
	if( node  == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
		
	for( p = node; p; p = p->next )
		if( fn_match( p->item, param ) )
			break;
			
	return p;
}

ListIterator
list_find_item_reverse( ListIterator node,  list_item_checker fn_match, void * param )
{
	ListIterator p;
	
	if( node  == NULL )
		ooc_throw( exception_new( err_wrong_position ) );

	for( p = node; p; p = p->previous )
		if( fn_match( p->item, param ) )
			break;
			
	return p;
}
