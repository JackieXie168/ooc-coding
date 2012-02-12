
#ifndef XMLNODE_H
#define XMLNODE_H

#include <ooc/list.h>

DeclareClass( XmlNode, ListNode );

XmlNode xmlnode_new( XmlNode parent );

void	xmlnode_write( XmlNode, XmlWriter );

typedef
enum _XmlNodeTypes
{
	XML_NODE_UNDEFINED = 0,

	XML_NODE_DOCUMENT,
	XML_NODE_ELEMENT,
	XML_NODE_ATTR,
	XML_NODE_TEXT,
	XML_NODE_CDATA,
	XML_NODE_COMMENT,

	XML_NODE_TYPES_MAX
} XmlNodeTypes;

#endif  /* XMLNODE_H */
