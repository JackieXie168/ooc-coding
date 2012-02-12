
/* This is a XmlBase class implementation file
 */

#include "xml.h"
#include "implement/xml.h"

#include <ooc/implement/exception.h>
#include <ooc/vector.h>

#include <string.h>

AllocateClass( XmlBase, Base );

static void XmlBase_initialize( Class this ) {}

#ifndef OOC_NO_FINALIZE
static void XmlBase_finalize( Class this ) {}
#endif

static void XmlBase_constructor( XmlBase self, const void * params ) {}
static void XmlBase_destructor( XmlBase self, XmlBaseVtable vtab ) {}
static int	XmlBase_copy( XmlBase self, const XmlBase from ) { 	return OOC_COPY_DEFAULT; }

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

Exception
xmlexception_new( enum XmlErrorCodes error )
{
	ooc_init_class( XmlException );
		
	return (Exception) ooc_new( XmlException, & error );
}

/** @class XmlAttribs
 *  @brief XmlAttribs class - brief description.
 * 	@see xml.h
 */

/** @brief XmlAttribs class - brief description.
 * XmlAttribs is a .....
 * @note	This class is a final class, can not be inherited.
 */ 

/* Declaring the members of the class. 
 * They are all private.
 */

Virtuals( XmlAttribs, Base )
EndOfVirtuals;

ClassMembers( XmlAttribs, Base )

	int		data;

EndOfClassMembers;

AllocateClass( XmlAttribs, Base );

static
void
XmlAttribs_initialize( Class this )
{
	XmlAttribsVtable vtab = & XmlAttribsVtableInstance;
	
	ooc_init_class( Vector );
}

static
void
XmlAttribs_finalize( Class this )
{
}

static
void
XmlAttribs_constructor( XmlAttribs self, const void * params )
{
	assert( ooc_isInitialized( XmlAttribs ) );
	
	chain_constructor( XmlAttribs, self, NULL );

}

/* Destructor
 */

static
void
XmlAttribs_destructor( XmlAttribs self, XmlAttribsVtable vtab )
{
}

static
int
XmlAttribs_copy( XmlAttribs self, const XmlAttribs from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
	
	/* Copies data by hand */
	self->data = from->data;
	...
	return OOC_COPY_DONE;
	
	/* Prevent object duplication */
	return OOC_NO_COPY;
}

XmlAttribs
xmlattribs_new( void )
{
	return ooc_new( XmlAttribs, NULL );
}
