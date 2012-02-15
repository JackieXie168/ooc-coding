
/* This is a XmlNode class implementation file
 */

#include <string.h>

#include "xmlnode.h"

#include <ooc/exception.h>
#include <ooc/implement/list.h>



/** @class XmlNode
 *  @brief XmlNode class - brief description.
 * 	@see xmlnode.h
 */

/** @file xmlnode.h 
 *  @brief XmlNode class - brief description.
 * XmlNode is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

/* Declaring the members of the class. 
 * They are all private.
 */

ClassMembers( XmlNode, ListNode )

	XmlNodeTypes		type;
	char *				name;
	XmlNode				parent;
	Vector				children;
	char *				value;

EndOfClassMembers;

Virtuals( XmlNode, ListNode )
	Interface( Xml );
EndOfVirtuals;

InterfaceRegister( XmlNode )
{
	AddInterface( XmlNode, Xml )
};

AllocateClassWithInterface( XmlNode, ListNode );

static void XmlNode_populate( Xml );

static
void
XmlNode_initialize( Class this )
{
	XmlNodeVtable vtab = & XmlNodeVtableInstance;
	
	ooc_init_class( Vector );

	XmlNode_populate( & vtab->Xml );
}

/* Class finalizing
 */

static
void
XmlNode_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
XmlNode_constructor( XmlNode self, const void * params )
{
	assert( ooc_isInitialized( XmlNode ) );
	
	chain_constructor( XmlNode, self, NULL );

	if( params )
		self->parent = ooc_cast( (void*) params, XmlNode );
}

static
void
XmlNode_destructor( XmlNode self, XmlNodeVtable vtab )
{
	ooc_delete_and_null( (Object*) & self->children );
	ooc_free_and_null( (void**) & self->name );
	ooc_free_and_null( (void**) & self->value );
}

/* Copy constuctor
 */

static
int
XmlNode_copy( XmlNode self, const XmlNode from )
{
	self->type		= from->type;
	self->parent	= from->parent;
	self->name		= ooc_strdup( from->name );
	self->value		= ooc_strdup( from->value );
	self->children	= (Vector) ooc_duplicate( (Object) from->children );
	
	return OOC_COPY_DONE;
}

/*	=====================================================
	Class member functions
 */


XmlNode
xmlnode_new( XmlNode parent )
{
	ooc_init_class( XmlNode );
		
	return ooc_new( XmlNode, parent );
}

static
void
xmlnode_set_name( XmlNode self, const char * name )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );
	
	self->name = strdup( name );
}

static
void
xmlnode_set_type( XmlNode self, XmlNodeTypes type )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	if( type <= XML_NODE_UNDEFINED && type >= XML_NODE_TYPES_MAX )
		ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );

	self->type = type;
}

static
void
xmlnode_set_value( XmlNode self, const char * value )
{
	assert( ooc_isInstanceOf( self, XmlNode ) );

	self->value = ooc_strdup( value );
}

#define XML_NODE_CHILDREN_CHUNK_SIZE 8

static
void
xmlnode_add_child( XmlNode self, XmlNode child )
{
	ooc_manage_object( child );

	assert( ooc_isInstanceOf( self, XmlNode ) );
	assert( ooc_isInstanceOf( child, XmlNode ) );

	if( self->children == NULL )
		self->children = vector_new_type( XML_NODE_CHILDREN_CHUNK_SIZE, XmlNode, OOC_MANAGE );

	vector_push_back( self->children, ooc_pass( child ) );
}

/* Xml interface callbacks
 */

static
XmlReadState
xmlnode_read_beginElement( Object _self, XmlParser parser, XmlReadState state, const char * name, XmlAttribs )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	assert( state == 0 );

	xmlnode_set_name( self, name );
	/* TODO: adding all attributes */

	return 0;
}

static
void
xmlnode_read_text( Object _self, XmlParser parser, XmlReadState state, const char * text )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	XmlNode child = xmlnode_new( self );
	ooc_manage_object( child );
	xmlnode_set_type( child, XML_NODE_TEXT );
	xmlnode_set_value( child, text ? text : "" );
	xmlnode_add_child( self, ooc_pass( child ) );
}

static
void
xmlnode_read_comment( Object _self, XmlParser parser, XmlReadState state, const char * comment )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	XmlNode child = xmlnode_new( self );
	ooc_manage_object( child );
	xmlnode_set_type( child, XML_NODE_COMMENT );
	xmlnode_set_value( child, comment ? comment : "" );
	xmlnode_add_child( self, ooc_pass( child ) );
}

static
void
xmlnode_write_children( Object _self, XmlWriter xmlw )
{
	XmlNode self = ooc_cast( _self, XmlNode );

	if( self->children )
	{
		vector_foreach( self->children, (vector_item_executor) xmlnode_write, xmlw );
	}
	else
		xml_write_text( xmlw, self->value );
}

static
void
xmlnode_write( XmlNode self, XmlWriter xmlw )
{
	switch( self->type )
	{
		case XML_NODE_DOCUMENT	:	/* xml_write_prolog( xmlw ); */
									xmlnode_write_children( self, xmlw );
									/* xml_write_epilog( xmlw ); */
									break;
		case XML_NODE_ELEMENT	:	xml_write_begin_element( xmlw, self->name );
									xmlnode_write_children( self, xmlw );
									xml_write_end_element();
									break;
		case XML_NODE_ATTR		:	xml_write_attribute( xmlw, self->name, self->value );
									break;
		case XML_NODE_TEXT		:	xml_write_text( xmlw, self->value );
									break;
		case XML_NODE_CDATA		:	/* TODO: CDATA write implementation */
									break;
		case XML_NODE_COMMENT	:	xml_write_comment( xmlw, self->value );
									break;
		default : ooc_throw( xmlexception_new( XML_ERROR_IMPLEMENTATION ) );
	}
}

static
void
XmlNode_populate( Xml xml )
{
	xml->on_read_beginElement	= xmlnode_read_beginElement;
	xml->on_read_comment		= xmlnode_read_comment;
	xml->on_read_text			= xmlnode_read_text;

	xml->on_read_child			= NULL;
	xml->on_read_endElement 	= NULL;

	xml->on_write_begin			= xmlnode_write;
	xml->on_write_attributes	= NULL;
	xml->on_write_data			= NULL;
	xml->on_write_end			= NULL;
}

