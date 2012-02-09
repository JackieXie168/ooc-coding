
/* This is a XmlBase class implementation file
 */

#include "xml.h"
#include "implement/xml.h"

#include <ooc/implement/exception.h>

#include <string.h>

AllocateClass( XmlBase, Base );

static void XmlBase_initialize( Class this ) {}

#ifndef OOC_NO_FINALIZE
static void XmlBase_finalize( Class this ) {}
#endif

static void XmlBase_constructor( XmlBase self, const void * params ) {}
static void XmlBase_destructor( XmlBase self, XmlBaseVtable vtab ) {}
static int	XmlBase_copy( XmlBase self, const XmlBase from ) { 	return OOC_COPY_DEFAULT; }

/** @class XmlException
 *  @brief XmlException class - brief description.
 * 	@see xmlexception.h
 */

/** @file xmlexception.h 
 *  @brief XmlException class - brief description.
 * XmlException is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlException, Exception )

EndOfClassMembers;

Virtuals( XmlException, Exception )
EndOfVirtuals;

AllocateClass( XmlException, Exception );

static void XmlException_initialize( Class this ) {}
static void XmlException_finalize( Class this ) {}

static
void
XmlException_constructor( XmlException self, const void * params )
{
	assert( ooc_isInitialized( XmlException ) );
	
	chain_constructor( XmlException, self, NULL );

    if( params )
        self->Exception.user_code = * ( (int*) params );
}

static void XmlException_destructor( XmlException self, XmlExceptionVtable vtab ) {}

static
int
XmlException_copy( XmlException self, const XmlException from )
{
	return OOC_COPY_DEFAULT;
}

Exception
xmlexception_new( enum XmlErrorCodes error )
{
	ooc_init_class( XmlException );
		
	return (Exception) ooc_new( XmlException, & error );
}


DeclareClass( _XmlNodeValue, ListNode );

ClassMembers( _XmlNodeValue, ListNode )
	char *		name;
	char *		value;
EndOfClassMembers;

Virtuals( _XmlNodeValue, ListNode )
EndOfVirtuals;

AllocateClass( _XmlNodeValue, ListNode );

static	void	_XmlNodeValue_initialize( Class this ) {}
static	void	_XmlNodeValue_finalize( Class this ) {}

struct _XmlNodeValueConstParam
{
	const char * name;
	const char * value;
};

static	void	_XmlNodeValue_constructor( _XmlNodeValue self, const void * params )
{
	struct _XmlNodeValueConstParam * p = (struct _XmlNodeValueConstParam *) params;

	assert( ooc_isInitialized( _XmlNodeValue ) );

	chain_constructor( _XmlNodeValue, self, NULL );

	self->name	= p->name ? strdup( p->name ) : NULL;
	self->value = p->value ? strdup( p->value ) : NULL;
}

static	void	_XmlNodeValue_destructor( _XmlNodeValue self, _XmlNodeValueVtable vtab )
{
	ooc_free_and_null( (void**) & self->name );
	ooc_free_and_null( (void**) & self->value );
}

static	int	_XmlNodeValue_copy( _XmlNodeValue self, const _XmlNodeValue from )
{
	self->name	= from->name ? strdup( from->name ) : from->name;
	self->value = from->value ? strdup( from->value ) : from->value;
	return OOC_COPY_DONE;
}

static
_XmlNodeValue
xmlnodevalue_new( const char * name, const char * value )
{
	struct _XmlNodeValueConstParam p;

	p.name	= name;
	p.value = value;

	return ooc_new( _XmlNodeValue, & p );
}

/** @class XmlNode
 *  @brief XmlNode class - brief description.
 * 	@see xml.h
 */

/** @file xmlnode.h 
 *  @brief XmlNode class - brief description.
 * XmlNode is a .....
 */ 

/* Allocating the class description table and the vtable
 */

InterfaceRegister( XmlNode )
{
	AddInterface( XmlNode, Xml )
};

AllocateClassWithInterface( XmlNode, ListNode );

static void XmlNode_populate( Xml xml );

static
void
XmlNode_initialize( Class this )
{
	XmlNodeVtable vtab = & XmlNodeVtableInstance;
	
	XmlNode_populate( & vtab->Xml );

	ooc_init_class( List );
	ooc_init_class( ListNode );
	ooc_init_class( _XmlNodeValue );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
XmlNode_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
XmlNode_constructor( XmlNode self, const void * params )
{
	assert( ooc_isInitialized( XmlNode ) );
	
	chain_constructor( XmlNode, self, NULL );

	self->name = strdup( params );
	self->children = list_new_type( XmlNode, OOC_MANAGE );
	self->values = list_new_type( _XmlNodeValue, OOC_MANAGE );
	self->text = list_new( ooc_free );
}

/* Destructor
 */

static
void
XmlNode_destructor( XmlNode self, XmlNodeVtable vtab )
{
	ooc_free_and_null( (void**) & self->name );
	ooc_delete_and_null( (Object*) & self->children );
	ooc_delete_and_null( (Object*) & self->values );
	ooc_delete_and_null( (Object*) & self->text );
}

/* Copy constuctor
 */

static
int
XmlNode_copy( XmlNode self, const XmlNode from )
{
	self->name		= strdup( from->name );
	self->children	= (List) ooc_duplicate( (Object) from->children );
	self->values	= (List) ooc_duplicate( (Object) from->values );
	self->text		= (List) ooc_duplicate( (Object) from->text );
	return OOC_COPY_DONE;
}

/*	=====================================================
	Class member functions
 */


XmlNode
xmlnode_new( const char * name )
{
	ooc_init_class( XmlNode );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	return ooc_new( XmlNode, name );
}

static
void
xmlnode_read_child( Object _self, Object child )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	list_append( self->children, child );
}

static
void
xmlnode_read_value( Object _self, const char * name, const char * value )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	list_append( self->values, xmlnodevalue_new( name, value ) );
}

static
void
xmlnode_read_text( Object _self, const char * text )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	list_append( self->text, strdup( text ) );
}

static
void
XmlNode_populate( Xml xml )
{
	xml->on_read_comment	= NULL;
	xml->on_read_value		= xmlnode_read_value;
	xml->on_read_text		= xmlnode_read_text;
	xml->on_read_child		= xmlnode_read_child;
}
