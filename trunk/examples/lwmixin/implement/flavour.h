#ifndef IMPL_FLAVOUR_H
#define IMPL_FLAVOUR_H

#ifndef FLAVOUR_H
#error Do #include "flavour.h" right before doing #include "implement/flavour.h"
#endif

typedef
struct
_FlavourDataFields
{
	const char *		flavour;

} FlavourDataFields;

extern	void	Flavour_populate( Flavour );

extern	void	Flavour_constructor( FlavourData, const char * flavour );
extern	void	Flavour_destructor( FlavourData );


#define FlavourData( pObject ) ( (FlavourData) & (pObject)->Flavour )

#endif /* IMPL_FLAVOUR_H */
