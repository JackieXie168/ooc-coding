#ifndef FLAVOUR_H
#define FLAVOUR_H

#include <ooc/ooc.h>

DeclareMixin( Flavour )

	void 			(* set 			)( Object, const char * flavour );
	const char * 	(* taste 		)( Object );

EndOfMixin;

#endif  /* FLAVOUR_H */
