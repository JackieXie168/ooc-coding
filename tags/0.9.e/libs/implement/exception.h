
#ifndef	IMPL_EXCEPTION_H
#define IMPL_EXCEPTION_H

#include "../exception.h"

/** Exception class data members.
 * These members are protected members, so the derived classes
 * can access them.
 */
 
ClassMembers( Exception, Base )

	int		code;			/**< The error code (used only by the Exception thrown by the ooc core) */
	int     user_code;		/**< Can be used for error codes in derived exception classes.
								 Using user_code can simplify the use of derived classes. */

EndOfClassMembers;

#endif /* IMPL_EXCEPTION_H */
