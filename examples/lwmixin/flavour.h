#ifndef FLAVOUR_H
#define FLAVOUR_H

#include <ooc/ooc.h>

typedef struct _FlavourDataFields * FlavourData;

DeclareInterface( Flavour )

	FlavourData	(*_get_data_)	( Object );

	void 			(* set 			)( Object, const char * flavour );
	const char * 	(* taste 		)( Object );

EndOfInterface;

#endif  /* FLAVOUR_H */
