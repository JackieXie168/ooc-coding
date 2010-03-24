
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


/** @class ListNode
 *  @brief ListNode class - brief description.
 * The ListNode objects forms the chain of the List. It has two pointers: one for the previous and one 
 * for the next ListNode in the List.
 * You can create a more effective List, if your items are subclasses of ListNode class, and you create
 * the List with list_new_of_nodes(). In this case your data are stored whithin the node, thus less @c malloc()
 * is called, and less space is used.
 * Unlike "noded" Lists, normal Lists hold only a pointer in the ListNode to the data you put in the List, that 
 * needs an additional memory allocation for the ListNode.
 * 	@see listnode.h
 */

AllocateClass( ListNode, Base );

static
void
ListNode_initialize( Class this )
{
}

static
void
ListNode_finalize( Class this )
{
}

static
void
ListNode_constructor( ListNode self, const void * params )
{
}

static
void
ListNode_destructor( ListNode self )
{
}

static
int
ListNode_copy( ListNode self, const ListNode from )
{
	/* This ListNode is not a member of any List! */
	/* both chain pointers get to be zeroed */
	return OOC_COPY_DONE;
}

/* ListNode for general list items
 */
 
DeclareClass( _ListNodeVoidp, ListNode );

Virtuals( _ListNodeVoidp, ListNode )
EndOfVirtuals;

ClassMembers( _ListNodeVoidp, ListNode )

	void *		item;
	
EndOfClassMembers;

#define get_item_ptr(x) (((_ListNodeVoidp)x)->item)

AllocateClass( _ListNodeVoidp, ListNode  );

static
void
_ListNodeVoidp_initialize( Class this )
{
}

static
void
_ListNodeVoidp_finalize( Class this )
{
}

static
void
_ListNodeVoidp_constructor( _ListNodeVoidp self, const void * params )
{
}

static
void
_ListNodeVoidp_destructor( _ListNodeVoidp self )
{
	/* We do not destroy the item here, because we do not know
	 * the destroyer for the item here!
	 * Only List knows it, so List manages the item as well! */
}

static
int
_ListNodeVoidp_copy( _ListNodeVoidp self, const _ListNodeVoidp from )
{
	return OOC_COPY_DEFAULT;
}

/************************************************
 * List class preparation
 */

static
void
List_initialize( Class this )
{
	ooc_init_class( _ListNodeVoidp );
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
		
		if( (! self->list_of_nodes) && self->destroy )
			self->destroy( get_item_ptr(p) );
		
		ooc_delete( (Object) p );
		}	
}

/* Copy constuctor
 */

static
int
List_copy( List self, const List from )
{
	return OOC_NO_COPY;
}

/*	=====================================================
	List chain / unchain helpers
 */

static
ListNode
create_new_item( List self, const void * new_item )
{
	ListNode tmp;

	if( self->list_of_nodes )
		tmp = (ListNode) new_item;
	else {
		tmp = (ListNode) ooc_new( _ListNodeVoidp, NULL );
		get_item_ptr(tmp) = (void * ) new_item;
		}

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
	return (List) ooc_new( List, destroyer );
}

List
_list_new_type( Class type, int manage )
{
	list_item_destroyer destroyer;
	List self;
	
	if( ! _ooc_isClassOf( type, & BaseClass ) )
		ooc_throw( exception_new( err_bad_cast ) );
		
	destroyer = manage ? (list_item_destroyer) ooc_delete : NULL ;
	
	self = list_new( destroyer );
	
	self->type = type;
	
	return self;
}

List
_list_new_of_nodes( Class node, int manage )
{
	List self;
	
	if( ! _ooc_isClassOf( node, & ListNodeClass ) )
		ooc_throw( exception_new( err_bad_cast ) );
		
	self = _list_new_type( node, manage );
	
	self->list_of_nodes = TRUE;
	
	return self;
}


ListIterator
list_append( List self, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	 
	if( self->type )
		_ooc_check_cast( new_item, self->type );
		
	if( self->last == NULL )
	
		return chain_first_item( self, create_new_item( self, new_item ) );
		
	else
	
		return chain_after( self, self->last, create_new_item( self, new_item ) );
		
}

ListIterator
list_prepend( List self, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	 
	if( self->type )
		_ooc_check_cast( new_item, self->type );
		
	if( self->first == NULL )
	
		return chain_first_item( self, create_new_item( self, new_item ) );
		
	else
	
		return chain_before( self, self->first, create_new_item( self, new_item ) );	
}

ListIterator
list_insert_before( List self, ListIterator location, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	 
	if( self->type )
		_ooc_check_cast( new_item, self->type );
		
	if( location == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
	 
	if( self->first == NULL )
	
		return chain_first_item( self, create_new_item( self, new_item ) );
		
	else
	
		return chain_before( self, location, create_new_item( self, new_item ) );
}

ListIterator
list_insert_after( List self, ListIterator location, void * new_item )
{
	assert( ooc_isInstanceOf( self, List ) );
	
	if( self->type )
		_ooc_check_cast( new_item, self->type );
		
	if( location == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
	 
	if( self->first == NULL )
	
		return chain_first_item( self, create_new_item( self, new_item ) );
		
	else
	
		return chain_after( self, location, create_new_item( self, new_item ) );
}

void *
list_remove_item( List self, ListIterator location )
{
	ListNode		removed_node;
	void *			item;

	assert( ooc_isInstanceOf( self, List ) );
	
	if( location == NULL || self->first == NULL )
		ooc_throw( exception_new( err_wrong_position ) );

	removed_node = unchain( self, location );
	
	if( self->list_of_nodes )
		item = removed_node;
	else {
		item = get_item_ptr( removed_node );
		ooc_delete( (Object) removed_node );
		}

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
	
	if( node->Base._vtab->_class == & _ListNodeVoidpClass )
		return get_item_ptr(node);
	else
		return node;
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
	ListNode tmp_next, tmp_prev;
	
	if( node1 == NULL || node2 == NULL )
		ooc_throw( exception_new( err_wrong_position ) );
	
	tmp_next 		= node1->next;
	tmp_prev 		= node1->previous;
	node1->next 	= node2->next;
	node1->previous = node2->previous;
	node2->next 	= tmp_next;
	node2->previous = tmp_prev;
}

void
list_foreach( List self, list_item_executor func, void * param )
{
	ListIterator p;
	
	assert( ooc_isInstanceOf( self, List ) );

	for( p = self->first; p; p = p->next ) 
		func( list_get_item( p ), param ); 
}

ListIterator
list_foreach_until_true( List self, ListIterator from, list_item_checker func, void * param )
{
	ListIterator p;
	
	assert( ooc_isInstanceOf( self, List ) );

	for( p = from ? from : self->first; p; p = p->next )
		if( func( list_get_item( p ), param ) )
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

		if( func( list_get_item( act ), param ) )
			list_delete_item( self, act );
		}
}


ListIterator
list_find_item( ListIterator node, list_item_checker fn_match, void * param )
{
	ListIterator p;
	
	for( p = node; p; p = p->next )
		if( fn_match( list_get_item( p ), param ) )
			break;
			
	return p;
}

ListIterator
list_find_item_reverse( ListIterator node,  list_item_checker fn_match, void * param )
{
	ListIterator p;
	
	for( p = node; p; p = p->previous )
		if( fn_match( list_get_item( p ), param ) )
			break;
			
	return p;
}
