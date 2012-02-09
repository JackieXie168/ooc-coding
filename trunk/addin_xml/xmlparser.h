
#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <stdio.h>

DeclareClass( XmlParser, XmlBase );

XmlParser xmlparser_new_file( FILE * );
XmlParser xmlparser_new_str( char * str );

Object xmlparser_parse( XmlParser self );

/* Virtual function definitions
 */

#endif  /* XMLPARSER_H */
