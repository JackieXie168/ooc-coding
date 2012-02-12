
#ifndef OOC_XML_H
#define OOC_XML_H

#include <stdio.h>

#include <ooc/ooc.h>
#include <ooc/exception.h>

DeclareClass( XmlBase, Base );

#include "xmlwriter.h"
#include "xmlparser.h"

typedef int XmlReadState;
DeclareClass( XmlAttribs, Base );

DeclareInterface( Xml )

	/* XmlWriter callback methods */
	const char *	(* on_write_name )		( Object, XmlWriter );
	void 			(* on_write_attributes )( Object, XmlWriter );
	void 			(* on_write_data )		( Object, XmlWriter );

	/* XmlParser callback methods */

	XmlReadState	(* on_read_beginElement)( Object, XmlParser, XmlReadState, const char * name, XmlAttribs );
	void			(* on_read_endElement)	( Object, XmlParser, XmlReadState );
	void			(* on_read_text )		( Object, XmlParser, XmlReadState, const char * text );
	void			(* on_read_child )		( Object, XmlParser, XmlReadState, Object child );
	void			(* on_read_comment )	( Object, XmlParser, XmlReadState, const char * comment );

	/*
	void		(* on_read_name )		( Object, const char * name );
	void		(* on_read_value )		( Object, const char * name, const char * value );
	void		(* on_read_text )		( Object, const char * text );
	void		(* on_read_child )		( Object, Object child );
	void		(* on_read_comment )	( Object, const char * comment );
	*/

EndOfInterface;

/* Xml class registration
 */

struct _XmlClassRegisterItem
{
	const Class						class;
	int								context;
	ROM char *						name;
	struct _XmlClassRegisterItem *	next;
};

void _xml_register_class( struct _XmlClassRegisterItem * );

#define xml_register_class( pClass, pContext, pName )			\
do { 															\
	static struct _XmlClassRegisterItem xmlClass;				\
	xmlClass.class = & pClass ## Class;							\
	xmlClass.context = pContext;								\
	xmlClass.name = pName;										\
	xmlClass.next = NULL; 										\
	_xml_register_class( &xmlClass ); 							\
} while(0)

/* XmlException
*/

enum XmlErrorCodes
{
	XML_ERROR_NONE = 0,
	XML_ERROR_BUFFER_FULL,
	XML_ERROR_DIRECTION,
	XML_ERROR_FILE_ASSIGNMENT,
	XML_ERROR_PARAMETER,
	XML_ERROR_SEQUENCE,
	XML_ERROR_IO,

	XML_ERROR_IMPLEMENTATION,
};

DeclareClass( XmlException, Exception );

/* usefull tools
 */

# define ooc_strdup( x ) ( x ? strdup(x) : x )

#endif  /* OOC_XML_H */
