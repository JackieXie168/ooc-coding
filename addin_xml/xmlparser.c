
/* This is a XmlParser class implementation file
 */


#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include <ctype.h>

#include "xml.h"
#include "implement/xml.h"

#define VALUE_STACK_SIZE 64

typedef
enum _Symbol
{
	/* Recognized symbols.
	 * These symbols can be recognized in the stream.
	 */
	end,
	space = ' ',
	quot = '"',
	apos = '\'',
	eqSign = '=',
	tagStart, tagStop,
	tagStartClose, tagEmptyClose,
	commentStart, commentStop,
	xmlDeclStart, xmlDeclStop,

	/* unrecognized symbols */
	characters = 0x100,	/* unrecognized symbols, can be any kind of character stream */

	/* Unrecognized symbols that can be checked against a rule.
	 * These symbols do NOT place a value onto the value stack.
	 */
	comment,

	/* Unrecognized symbols that can be checked against a rule.
	 * These symbols place a value onto the value stack.
	 */
	bom,
	xmlDeclXML,
	name,
	escapedLiteral,

	/* Symbols that resulted from a reduction.
	 * These symbols have a corresponding value on the value stack
	 * and are status variables as well.
	 */
	tagName,	/*   */
	tagObject,	/*   */

} Symbol;

typedef int boolean;
#define false 0
#define true (!false)

typedef
struct SymbolValueItem
{
	const char *	location;
	int				length;
}
SymbolValue;

typedef
struct ObjectValueItem
{
	Object			object;
	Xml				xml;
}
ObjectValue;

typedef
struct ValueStackItem
{
	Symbol			symbol;

	union
	{
	SymbolValue		symbolValue;
	ObjectValue		xmlObject;
	} v;
}
Value;

ClassMembers( XmlParser, XmlBase )

	char *				buffer;
	char *				next_pt;
	Symbol				sym;
	Symbol				string_delimiter;
	Value *				vs;
	int					vsp;

EndOfClassMembers;

/* Allocating the class description table and the vtable
 */

Virtuals( XmlParser, XmlBase )
EndOfVirtuals;

AllocateClass( XmlParser, XmlBase );

static
void
XmlParser_initialize( Class this )
{
}

static
void
XmlParser_finalize( Class this )
{
}


/* Constructor
 */

static
void
XmlParser_constructor( XmlParser self, const void * params )
{
	assert( ooc_isInitialized( XmlParser ) );
	
	chain_constructor( XmlParser, self, NULL );

	self->vs = ooc_malloc( VALUE_STACK_SIZE * sizeof(Value) );
	self->vsp = -1;

	/* TODO: modifíy to be dynamic */
	self->buffer = "\t <?xml\tversion='1.0' encoding=\"UTF-8\" ?> <html><head/><body>body ez ám a javából</body><!-- \n  \rSome comment here \t   --></html>   ";
	self->next_pt = self->buffer;
}

static
void
XmlParser_destructor( XmlParser self, XmlParserVtable vtab )
{
	ooc_free_and_null( (void**) & self->vs );
	ooc_free_and_null( (void**) & self->buffer );
}

static
int
XmlParser_copy( XmlParser self, const XmlParser from )
{
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


XmlParser
xmlparser_new_str( char * str )
{
	XmlParser self;

	ooc_init_class( XmlParser );
		
	self = ooc_new( XmlParser, NULL );

	self->buffer = str;

	self->next_pt = self->buffer;

	return self;
}

#define p 		(self->next_pt)
#define	vs		(self->vs)
#define vsp		(self->vsp)
#define sym		(self->sym)

static
void
vs_push_symbol( XmlParser self, Symbol s, const char * pos, int len )
{
	assert( vsp < VALUE_STACK_SIZE - 1 );
	++vsp;
	vs[ vsp ].symbol = s;
	vs[ vsp ].v.symbolValue.location = pos;
	vs[ vsp ].v.symbolValue.length 	= len;

	p += len;
}

static
void
vs_drop_top_n( XmlParser self, int n )
{
	assert( n >= 1 );
	assert( vsp >= n-1 );

	vsp -= n;
							/* Should Objects be deleted ?
							 * No. They are passed to other Objects while reducing. */
}

#define vs_drop_top( self ) vs_drop_top_n( self, 1 )

static
void
error( const char * msg, ... )
{
	va_list 	args;

	va_start( args, msg );
	vprintf( msg, args );
	va_end (args);
}


/* The lexer
 */

static
boolean
check_name( XmlParser self )
{
	size_t len = 0;
	const char * c = p;

	/* name = ["a-zA-Z_"]["0-9:a-zA-Z_"]+ ;
	 * TODO: this is not good !!! the ':' must be handled correctly */

	if( isalpha( *c ) || *c == '_' )
	{
		len++; c++;
		while( isalnum( *c ) || *c == '_' || *c == ':' )
			len++, c++;
		vs_push_symbol( self, name, p, len );
		return true;
	}
	else
		return false;
}

static
boolean
check_escapedLiteral( XmlParser self )
{
	size_t len = 0;
	const char * c = p;

	/* escapedLiteral = [^<^>]* ; */

	while( *c != self->string_delimiter && *c != '\0' && *c != '<' && *c != '>' )
		len++, c++;

	if( len )
		vs_push_symbol( self, escapedLiteral, p, len );

	return len != 0;
}

static
boolean
check_xmlDeclXML( XmlParser self )
{
	if( toupper(*p) == 'X' && toupper(*(p+1)) == 'M' && toupper(*(p+2)) == 'L' ) {
		vs_push_symbol( self, xmlDeclXML, p, 3 );
		return true;
	}
	return false;
}

static
boolean
check_comment( XmlParser self )
{
	char * c = p;

	while( *c != '\0' && ( *c != '-' || *(c+1) != '-' || *(c+2) != '>' ) )
		c++;

	while( c != p && ( *(c-1) == ' ' || *(c-1) == '\t' || *(c-1) == '\n' || *(c-1) == '\r' ) )
		c--;

	p = c;

	return true;
}

static
void
getsym( XmlParser self )
{
	switch( *p++ ) {
	case '<' :	if		( *p == '?' ) 	{ p++; sym = xmlDeclStart; break; }
				else if ( *p == '/' ) 	{ p++; sym = tagStartClose; break; }
				else if ( *p == '!' && *(p+1) == '-' && *(p+2) == '-' )
										{ p+=3; sym = commentStart; break; }
				else					{      sym = tagStart; break; };
	case '/' :	if		( *p == '>' )	{ p++; sym = tagEmptyClose; break; };
				goto unrecognized;
	case '?' :	if		( *p == '>' )	{ p++; sym = xmlDeclStop; break; };
				goto unrecognized;
	case '>' :  sym = tagStop; break;
	case '-' :	if ( *p == '-' && *(p+1) == '>' )
										{ p+=2; sym = commentStop; break; };
				goto unrecognized;
	case '"' :	sym = quot; break;
	case '\'':	sym = apos; break;
	case '=' :  sym = eqSign; break;
	case ' ' :
	case '\t':
	case '\r':
	case '\n':	while( *p == ' ' || *p == '\t' || *p == '\n' || *p == '\r' )
					p++;
				sym = space; break;
	case '\0':	p--; sym = end; break;
	default	 :
	unrecognized: p--; sym = characters; break;
	}
}

static
boolean
checksym( XmlParser self, Symbol s )
{
	boolean retval = false;

	if( sym == characters )
	{
		switch( s ) {
			case name :				retval = check_name( self );
									break;
			case escapedLiteral : 	retval = check_escapedLiteral( self );
									break;
			case xmlDeclXML :		retval = check_xmlDeclXML( self );
									break;
			case comment :			retval = check_comment( self );
									break;
			default :
				break;
		}
	}
	return retval;
}


/* The parser
 */

static
boolean
accept( XmlParser self, Symbol s )
{
	boolean retval = true;

	if( sym == characters && s > characters )
		retval = checksym( self, s );
	else
		if( sym != s )
			return false;

	getsym( self );
	return retval;
}

static
boolean
expect( XmlParser self, Symbol s )
{
	if( accept( self, s ) )
		return true;
	error( "symbol %d expected\n", s );
	return false;
}

static
void
string( XmlParser self )
{
	if( ! accept( self, self->string_delimiter = quot ) )
		expect( self, self->string_delimiter = apos );
	expect( self, escapedLiteral );
	expect( self, self->string_delimiter );
}

static
void
reduceAttrib( XmlParser self )
{
	assert( vsp >= 2 );
	assert( vs[vsp].symbol == escapedLiteral );
	assert( vs[vsp-1].symbol == name );
	assert( vs[vsp-2].symbol == xmlDeclXML || vs[vsp-2].symbol == tagObject || vs[vsp-2].symbol == tagName );

	/* TODO: reduction here */
	vs_drop_top_n( self, 2 );
}

static
void
reduceText( XmlParser self )
{
	assert( vsp >= 1 );
	assert( vs[vsp].symbol == escapedLiteral );
	assert( vs[vsp-1].symbol == tagObject || vs[vsp-1].symbol == tagName );

	if( vs[vsp-1].symbol == tagObject ) {
		/* TODO: reduction here */
		vs_drop_top( self );
	}
}

static
void
reduceElement( XmlParser self )
{
	assert( vsp >= 1 );
	assert( vs[vsp].symbol == name );
	assert( vs[vsp-1].symbol == tagObject || vs[vsp-1].symbol == tagName || vs[vsp-1].symbol == escapedLiteral );

	if( vs[vsp-1].symbol == tagObject ) {
		/* TODO: reduction here */
		vs_drop_top( self );
	}
}

static
void
reduceEmptyElement( XmlParser self )
{
	assert( vsp >= 0 );
	assert( vs[vsp].symbol == tagObject || vs[vsp].symbol == tagName );

	/* TODO: reduction here */
	vs_drop_top( self );
}

static
void
attribList( XmlParser self )
{
	accept( self, space );
	while( accept( self, name ) ){
		accept( self, space );
		expect( self, eqSign ); accept( self, space );
		string( self ); accept( self, space );
		reduceAttrib( self );
	}
}

static
void
xmlDecl( XmlParser self )
{
	if( accept( self, xmlDeclStart ) ) {
		expect( self, xmlDeclXML );
		attribList( self );
		expect( self, xmlDeclStop );

		/* TODO: reduction here */
		assert( vs[vsp].symbol == xmlDeclXML );
		vs_drop_top( self );
	}
}

static
void
prolog( XmlParser self )
{
	 accept( self, space );
	 xmlDecl( self );
	 accept( self, space );
}

static
void
xmlElement( XmlParser self )
{
	while( true ) {
		accept( self, space );
		if( accept( self, tagStart ) ) {
			accept( self, space );
			expect( self, name );
			/* instantiate name */
			vs[ vsp ].symbol = tagName; /* if can not be instantiated */
			attribList( self );
			if( accept( self, tagEmptyClose ) )
				reduceEmptyElement( self );
			else {
				expect( self, tagStop );
			}
		}
		else if( accept( self, tagStartClose )) {
			accept( self, space );
			expect( self, name ); accept( self, space );
			expect( self, tagStop );
			reduceElement( self );
		}
		else if( accept( self, commentStart ) ) {
			accept( self, space );
			expect( self, comment );
			accept( self, space );
			expect( self, commentStop );
		}
		else if( accept( self, escapedLiteral ) ) {
			reduceText( self );
		}
		else
			break;
	}
}

static
void
xmlFile( XmlParser self )
{
	getsym( self );
	accept( self, bom );
	prolog( self );
	xmlElement( self);
	expect( self, end );
}

Object
xmlparser_parse( XmlParser self )
{
	xmlFile( self );

	assert( vsp == 0 );
	assert( vs[0].symbol == tagObject );

	return vs[0].v.xmlObject.object;
}
