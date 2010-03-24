
#ifndef LISTENER_H
#define LISTENER_H

#include <ooc.h>

DeclareClass( Listener, Base );

Listener listener_new( void );

void listener_methods( Listener self, int );

/* Virtual function definitions
 */

Virtuals( Listener, Base )

	void	(* listener_virtual)  ( Listener );

EndOfVirtuals;

#endif  /* LISTENER_H */
