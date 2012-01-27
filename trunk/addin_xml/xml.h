
#ifndef OOC_XML_H
#define OOC_XML_H

#include <stdio.h>

#include <ooc/ooc.h>
#include <ooc/exception.h>

enum XmlDirection
{
	XML_OUTPUT,
	XML_INPUT
};

DeclareClass( XmlManager, Base );

XmlManager 	xml_manager_new( enum XmlDirection direction );

void		xml_set_file( XmlManager, FILE * );
void		xml_set_indent( XmlManager, int );

enum XmlDirection xml_get_direction( XmlManager );
FILE *		xml_get_file( XmlManager );
int			xml_get_indent( XmlManager );

void		xml_write_begin_element( XmlManager, const char * name );
void		xml_write_end_element( XmlManager );
void		xml_write_attribute( XmlManager, const char * name, const char * value );
void		xml_write_text( XmlManager, const char * text );
void		xml_write_element_text( XmlManager, const char * name, const char * text );
void		xml_write_begin_comment( XmlManager );
void		xml_write_end_comment( XmlManager );
void		xml_write_comment( XmlManager, const char * );

DeclareInterface( Xml )

	void 	(* on_write_attributes )( Object, XmlManager );
	void 	(* on_write_data )( Object, XmlManager );

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


