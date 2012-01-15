
/* This is a XmlManager class implementation file
 */

#include "xml.h"

#include <ooc/list.h>

#include <ooc/implement/exception.h>

#include <stdarg.h>

static Exception xmlexception_new( enum XmlErrorCodes error );

/** @class XmlManager
 *  @brief XmlManager class - brief description.
 * 	@see xmlmanager.h
 */

/** @file xmlmanager.h 
 *  @brief XmlManager class - brief description.
 * XmlManager is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

enum XmlManagerState
{
	STATE_NOTHING = 0,
	STATE_BOM,
	STATE_VERSION,
	STATE_PROLOG,
	STATE_BEGIN_ELEMENT,
	STATE_ATTRIBUTES,
	STATE_TEXT,
	STATE_END_ELEMENT
};

ClassMembers( XmlManager, Base )

	enum XmlDirection			direction;

	FILE *						file;
	char *						buffer;
	size_t						buffer_length;

	enum XmlManagerState		state;
	List						elements;

	int							indent;
	int							indent_position;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

Virtuals( XmlManager, Base )
EndOfVirtuals;

AllocateClass( XmlManager, Base );

/* Class initializing
 */

static
void
XmlManager_initialize( Class this )
{
	ooc_init_class( List );
	ooc_init_class( XmlException );
}

/* Class finalizing
 */

static
void
XmlManager_finalize( Class this )
{
	/* Release global resources! */
}


/* Constructor
 */

static
void
XmlManager_constructor( XmlManager self, const void * direction )
{
	assert( ooc_isInitialized( XmlManager ) );
	
	chain_constructor( XmlManager, self, NULL );

	self->direction = * ((enum XmlDirection *) direction);

	if( self->direction != XML_OUTPUT && self->direction != XML_INPUT )
		ooc_throw( xmlexception_new( XML_ERROR_DIRECTION ) );

	self->elements 	= list_new( NULL );
	self->indent	= 1;
}

/* Destructor
 */

static
void
XmlManager_destructor( XmlManager self, XmlManagerVtable vtab )
{
	ooc_delete_and_null( (Object*) & self->elements );
}

/* Copy constuctor
 */

static
int
XmlManager_copy( XmlManager self, const XmlManager from )
{
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


XmlManager
xml_manager_new( enum XmlDirection direction )
{
	ooc_init_class( XmlManager );
		
	return ooc_new( XmlManager, & direction );
}

void
xml_set_file( XmlManager self, FILE * file )
{
	ooc_cast( self, XmlManager );

	if( self->file )
		ooc_throw( xmlexception_new( XML_ERROR_FILE_ASSIGNMENT ) );

	self->file = file;
}

void
xml_set_indent( XmlManager self, int indent )
{
	ooc_cast( self, XmlManager );

	if( self->direction != XML_OUTPUT )
		ooc_throw( xmlexception_new( XML_ERROR_DIRECTION ) );

	if( indent < -1 || indent > 16 )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	self->indent = indent;
}

static
void
xml_write( XmlManager self, const char * format, ... )
{
	va_list 	args;
	int			ret_val;

	if( self->file == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_FILE_ASSIGNMENT ) );

	va_start( args, format );
	ret_val = vfprintf( self->file, format, args );
	va_end (args);

	if( ret_val < 0 )
		ooc_throw( xmlexception_new( XML_ERROR_IO ) );
}

static
void
xml_write_nl( XmlManager self )
{
	if( self->indent >= 0 )
	{
		if( self->indent_position < 0 )
			self->indent_position = 0;

		if( self->indent_position )
			xml_write( self, "\n%*s", self->indent_position, "" );
		else
			xml_write( self, "\n" );
	}
}

static
void
xml_indent_inc( XmlManager self )
{
	if( self->indent > 0 )
	{
		self->indent_position += self->indent;
	}
}

static
void
xml_indent_dec( XmlManager self )
{
	if( self->indent > 0 )
	{
		self->indent_position -= self->indent;
	}
}

void
xml_write_begin_element( XmlManager self, const char * name )
{
	ooc_cast( self, XmlManager );

	if( self->state < STATE_PROLOG )
		self->state = STATE_PROLOG; /* TODO: Handling self->state < STATE_PROLOG */

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :
										xml_write( self, ">");
										xml_indent_inc( self );
		case STATE_TEXT :
		case STATE_END_ELEMENT :
										xml_write_nl( self );
		case STATE_PROLOG :
										xml_write( self, "<%s", name );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	list_append( self->elements, (char*) name );
	self->state = STATE_BEGIN_ELEMENT;
}

void
xml_write_end_element( XmlManager self )
{
	const char * name;

	ooc_cast( self, XmlManager );

	name = (char *) list_remove_last_item( self->elements );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			xml_write( self, "/>" );
										break;
		case STATE_TEXT :
		case STATE_END_ELEMENT :		xml_indent_dec( self );
										xml_write_nl( self );
										xml_write( self, "</%s>", name );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_END_ELEMENT;
}

void
xml_write_attribute( XmlManager self, const char * name, const char * value )
{
	ooc_cast( self, XmlManager );

	/* TODO: handling the NULL value attribute. (could be necessary for HTML :-( ) */

	if( name == NULL || value == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			xml_write( self, " %s=\"%s\"", name, value );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_ATTRIBUTES;
}

void
xml_write_text( XmlManager self, const char * text )
{
	ooc_cast( self, XmlManager );

	/* TODO: handling the NULL text element. (could be necessary for HTML :-( ) */

	if( text == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			xml_write( self, ">" );
										xml_indent_inc( self );
		case STATE_END_ELEMENT :		xml_write_nl( self );
		case STATE_TEXT :				xml_write( self, "%s", text );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_TEXT;
}

void
xml_write_element_text( XmlManager self, const char * name, const char * text )
{
	xml_write_begin_element( self, name );
	xml_write_text( self, text );
	xml_write_end_element( self );
}


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

static
Exception
xmlexception_new( enum XmlErrorCodes error )
{
	ooc_init_class( XmlException );
		
	return (Exception) ooc_new( XmlException, & error );
}
