
/* This is a XmlManager class implementation file
 */

#include "xml.h"

#include <ooc/list.h>

#include <ooc/implement/exception.h>

#include <stdarg.h>
#include <string.h>

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
	STATE_TEXT_NOINDENT,
	STATE_END_ELEMENT,
	STATE_BEGIN_COMMENT,
	STATE_COMMENT_TEXT,
	STATE_COMMENT_NOINDENT,
	STATE_END_COMMENT
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

enum
XmlDirection
xml_get_direction( XmlManager self )
{
	ooc_cast( self, XmlManager );

	return self->direction;
}

void
xml_set_file( XmlManager self, FILE * file )
{
	ooc_cast( self, XmlManager );

	if( self->file )
		ooc_throw( xmlexception_new( XML_ERROR_FILE_ASSIGNMENT ) );

	self->file = file;
}

FILE *
xml_get_file( XmlManager self )
{
	ooc_cast( self, XmlManager );

	return self->file;
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

int
xml_get_indent( XmlManager self )
{
	ooc_cast( self, XmlManager );

	return self->indent;
}

typedef
struct
EscapeBuffer
{
	char * 		converted;
	int			must_free;
} * EscapeBuffer;

#define ESCAPED_LT		"&lt;"
#define ESCAPED_GT		"&gt;"
#define ESCAPED_AMP		"&amp;"
#define ESCAPED_APOS	"&apos;"
#define ESCAPED_QUOT	"&quot;"

static
void
escape( EscapeBuffer buffer, const char * text )
{
	size_t text_len, escaped_len;
	const char * tp;
	char * ep;

	for( text_len = escaped_len = 0, tp = text; *tp; tp++, text_len++ )
		{
			/* TODO: handling UTF-8 sequences here */
			switch( *tp ) {
				case '<' :		escaped_len += sizeof( ESCAPED_LT )-1; break;
				case '>' :		escaped_len += sizeof( ESCAPED_GT )-1; break;
				case '&' :		escaped_len += sizeof( ESCAPED_AMP )-1; break;
				case '\'' :		escaped_len += sizeof( ESCAPED_APOS )-1; break;
				case '\"' :		escaped_len += sizeof( ESCAPED_QUOT )-1; break;
				default :		escaped_len++;
			}
		}
	if( text_len == escaped_len ) {
		buffer->converted = (char *) text;
		buffer->must_free = FALSE;
		}
	else {
		buffer->converted = ooc_malloc( escaped_len + 1 );
		buffer->must_free = TRUE;
		for( tp = text, ep = buffer->converted; *tp; tp++ )
			{
				/* TODO: handling UTF-8 sequences here */
				switch( *tp ) {
					case '<' :		strcpy( ep, ESCAPED_LT );
									ep += sizeof( ESCAPED_LT )-1;
									break;
					case '>' :		strcpy( ep, ESCAPED_GT );
									ep += sizeof( ESCAPED_GT )-1;
									break;
					case '&' :		strcpy( ep, ESCAPED_AMP );
									ep += sizeof( ESCAPED_AMP )-1;
									break;
					case '\'' :		strcpy( ep, ESCAPED_APOS );
									ep += sizeof( ESCAPED_APOS )-1;
									break;
					case '\"' :		strcpy( ep, ESCAPED_QUOT );
									ep += sizeof( ESCAPED_QUOT )-1;
									break;
					default :		*ep++ = *tp;
				}
			}
		*ep = '\0';
		}
}

static
void
escapebuffer_release( EscapeBuffer buffer )
{
	if( buffer->must_free == TRUE )
		ooc_free( buffer->converted );
}

static
void
xml_write_formatted( XmlManager self, const char * format, ... )
{
	va_list 	args;
	int			ret_val;

	if( self->file == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_FILE_ASSIGNMENT ) );

	va_start( args, format );
	ret_val = vfprintf( self->file, format, args );
	va_end (args);

	/* TODO: passing error information into the XmlException object */
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
			xml_write_formatted( self, "\n%*s", self->indent_position, "" );
		else
			xml_write_formatted( self, "\n" );
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

static
void
xml_write_prolog( XmlManager self )
{
	/* TODO: generating a proper XML prolog:
	 * BOM, version, encoding, doctype ... */

	xml_write_formatted(self, "%c%c%c", 0xEF, 0xBB, 0xBF ); /* Byte Order mark */

	xml_write_formatted( self, "<?xml version='1.0' encoding='UTF-8' ?>");

	self->state = STATE_PROLOG;
}

void
xml_write_begin_element( XmlManager self, const char * name )
{
	ooc_cast( self, XmlManager );

	if( self->state < STATE_PROLOG )
		xml_write_prolog( self );

	if( name == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :
										xml_write_formatted( self, ">");
										xml_indent_inc( self );
		case STATE_TEXT :
		case STATE_END_COMMENT :
		case STATE_END_ELEMENT :
		case STATE_PROLOG :				xml_write_nl( self );
										xml_write_formatted( self, "<%s", name );
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
		case STATE_ATTRIBUTES :			xml_write_formatted( self, "/>" );
										break;
		case STATE_TEXT :
		case STATE_END_COMMENT :
		case STATE_END_ELEMENT :		xml_indent_dec( self );
										xml_write_nl( self );
		case STATE_TEXT_NOINDENT :		xml_write_formatted( self, "</%s>", name );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_END_ELEMENT;
}

void
xml_write_attribute( XmlManager self, const char * name, const char * value )
{
	struct EscapeBuffer esc = { NULL, FALSE };

	ooc_manage( &esc, (ooc_destroyer) escapebuffer_release );

	ooc_cast( self, XmlManager );

	/* TODO: handling the NULL value attribute. (could be necessary for HTML :-( ) */

	if( name == NULL || value == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			escape( &esc, value );
										xml_write_formatted( self, " %s=\"%s\"", name, esc.converted );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_ATTRIBUTES;

	escapebuffer_release( ooc_pass( &esc ) );
}

void
xml_write_text( XmlManager self, const char * text )
{
	enum XmlManagerState next_state = STATE_TEXT;

	struct EscapeBuffer esc = { NULL, FALSE };

	ooc_manage( &esc, (ooc_destroyer) escapebuffer_release );

	ooc_cast( self, XmlManager );

	/* TODO: handling the NULL text element. (could be necessary for HTML :-( ) */

	if( text == NULL )
		ooc_throw( xmlexception_new( XML_ERROR_PARAMETER ) );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :			xml_write_formatted( self, ">" );
										xml_indent_inc( self );
		case STATE_END_COMMENT :
		case STATE_END_ELEMENT :		xml_write_nl( self );
		case STATE_TEXT :				escape( &esc, text );
										xml_write_formatted( self, "%s", esc.converted );
										break;

		case STATE_TEXT_NOINDENT :		escape( &esc, text );
										xml_write_formatted( self, ">%s", esc.converted );
										break;

		case STATE_BEGIN_COMMENT :		xml_indent_inc( self );
		case STATE_COMMENT_TEXT :		xml_write_nl( self );
		case STATE_COMMENT_NOINDENT :	xml_write_formatted( self, "%s", text );
										next_state = STATE_COMMENT_TEXT;
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = next_state;

	escapebuffer_release( ooc_pass( &esc ) );
}

void
xml_write_element_text( XmlManager self, const char * name, const char * text )
{
	xml_write_begin_element( self, name );
	self->state = STATE_TEXT_NOINDENT;
	xml_write_text( self, text );
	self->state = STATE_TEXT_NOINDENT;
	xml_write_end_element( self );
}

void
xml_write_begin_comment( XmlManager self )
{
	ooc_cast( self, XmlManager );

	if( self->state < STATE_PROLOG )
		xml_write_prolog( self );

	switch( self->state )
	{
		case STATE_BEGIN_ELEMENT :
		case STATE_ATTRIBUTES :
										xml_write_formatted( self, ">");
										xml_indent_inc( self );
		case STATE_TEXT :
		case STATE_END_ELEMENT :
		case STATE_END_COMMENT :
		case STATE_PROLOG :				xml_write_nl( self );
										xml_write_formatted( self, "<!-- " );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_BEGIN_COMMENT;
}

void
xml_write_end_comment( XmlManager self )
{
	ooc_cast( self, XmlManager );

	switch( self->state )
	{
		case STATE_BEGIN_COMMENT :
		case STATE_COMMENT_NOINDENT :	xml_write_formatted( self, " -->" );
										break;

		case STATE_COMMENT_TEXT :		xml_indent_dec( self );
										xml_write_nl( self );
										xml_write_formatted( self, "-->" );
										break;
		default :
			ooc_throw( xmlexception_new( XML_ERROR_SEQUENCE ) );
	}
	self->state = STATE_END_COMMENT;
}

void
xml_write_comment( XmlManager self, const char * comment )
{
	ooc_cast( self, XmlManager );

	xml_write_begin_comment( self );
	self->state = STATE_COMMENT_NOINDENT;
	xml_write_text( self, comment );
	self->state = STATE_COMMENT_NOINDENT;
	xml_write_end_comment( self );
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
