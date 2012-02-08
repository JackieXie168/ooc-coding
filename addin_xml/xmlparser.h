
#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <ooc/ooc.h>
#include "xml.h"

DeclareClass( XmlParser, Base );

XmlParser xmlparser_new( XmlManager xmlm );

Object xmlparser_parse( XmlParser self );

/* Virtual function definitions
 */

Virtuals( XmlParser, Base )
EndOfVirtuals;

#endif  /* XMLPARSER_H */
