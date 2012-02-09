
#ifndef IMPL_OOC_XML_H
#define IMPL_OOC_XML_H

#ifndef OOC_XML_H
#error Do #include "xml.h" right before doing #include "implement/xml.h"
#endif

#include <ooc/list.h>
#include <ooc/implement/list.h>

ClassMembers( XmlBase, Base )

EndOfClassMembers;

Virtuals( XmlBase, Base )

EndOfVirtuals;

/* Functions available for implementation only
 *
 */

Exception 		xmlexception_new( enum XmlErrorCodes error );

/* Default Escaped characters
 *
 */

#define ESCAPED_LT		"&lt;"
#define ESCAPED_GT		"&gt;"
#define ESCAPED_AMP		"&amp;"
#define ESCAPED_APOS	"&apos;"
#define ESCAPED_QUOT	"&quot;"

/* XmlNode :
 *
 */

ClassMembers( XmlNode, ListNode )

	char *			name;
	List			values;
	List			children;
	List			text;

EndOfClassMembers;

#endif /* IMPL_OOC_XML_H */
