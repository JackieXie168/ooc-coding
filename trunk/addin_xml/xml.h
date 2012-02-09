
#ifndef OOC_XML_H
#define OOC_XML_H

#include <stdio.h>

#include <ooc/ooc.h>
#include <ooc/exception.h>

DeclareClass( XmlBase, Base );

#include "xmlwriter.h"
#include "xmlparser.h"

DeclareInterface( Xml )

	/* XmlWriter callback methods */
	void 	(* on_write_attributes )( Object, XmlWriter );
	void 	(* on_write_data )		( Object, XmlWriter );

	/* XmlParser callback methods */
	void	(* on_read_comment )	( Object, const char * comment );
	void	(* on_read_value )		( Object, const char * name, const char * value );
	void	(* on_read_text )		( Object, const char * text );
	void	(* on_read_child )		( Object, Object child );

EndOfInterface;

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

#endif  /* OOC_XML_H */


