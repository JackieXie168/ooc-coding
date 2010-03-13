
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "ooc.h"

DeclareClass( Exception, Base );

enum
error_codes
{
	err_no_error		= 0,
	err_out_of_memory,					/* no memory for allocating an object */
	err_bad_throw,						/* bad use of throw (rethow whithout try, wrong code, etc.) */
	err_bad_cast,						/* bad cast of an object */
	err_undefined_virtual,				/* calling of an undefined virtual function */
	err_can_not_be_duplicated,			/* duplication of an object is invalid */
	err_wrong_position,					/* insertion at wrong position into a container */

	err_user_code						/* Firts code available for the users */
};

/* Class methods */

Object	exception_new( enum error_codes );
int		exception_get_error_code( const Exception );
int		exception_get_subcode( const Exception );

/* Virtual function definitions
 */

Virtuals( Exception, Base )

	/* No virtuals, yet */

EndOfVirtuals;

#endif  /* EXCEPTION_H */

