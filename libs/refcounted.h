#ifndef REFCOUNTED_H_
#define REFCOUNTED_H_

#include "ooc.h"

DeclareClass( RefCounted, Base );

/*   Class methods
 */
 
RefCounted refcnt_addref( RefCounted );


Virtuals( RefCounted, Base )

	/* No virtuals */

EndOfVirtuals;

#endif /*REFCOUNTED_H_*/
