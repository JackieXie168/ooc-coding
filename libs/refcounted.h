#ifndef REFCOUNTED_H_
#define REFCOUNTED_H_

#include "ooc.h"

DeclareClass( RefCounted, Base );

/*   Class methods
 */

/** Increments the reference counter.
 * @param	object	The Object to be counted.
 * @return	The object.
 * @note	To decrement the reference counter, simply ooc_delete() the object.
 * ooc_delete() decrements the counter, and does not delete it until zero.
 * The last ooc_delete will destroy the object.
 * @see refcnt_inc()
 */
  
RefCounted refcnt_addref( RefCounted object );

/** Increments the reference counter.
 * This is a macro version, which is more convenient since there is no need
 * for manual type casting.
 * @param	object	The Object to be counted.
 * @return	The object.
 * @see refcnt_addref()
 * @warning	Pass a pure Object pointer only! This macro has a side effect otherwise!
 * (e.g. passing a function as a parameter that returns the object is called twice!) 
 */
 
#define refcnt_inc( object ) ( refcnt_addref( (RefCounted) object ), object )

Virtuals( RefCounted, Base )

	/* No virtuals */

EndOfVirtuals;

#endif /*REFCOUNTED_H_*/
