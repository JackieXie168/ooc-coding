
/* This is a Flavour interface (light-weight mixin) implementation file
 */

#include "flavour.h"
#include "implement/flavour.h"

#include <ooc/exception.h>


AllocateInterface( Flavour );

/* Construction, destruction
 */

void
Flavour_constructor( FlavourData self, const char * flavour )
{
	self->flavour = flavour;
}

void
Flavour_destructor( FlavourData self )
{

}


/* Method implementations
 */

static
void
flavour_set( Object obj, const char * flavour )
{
	FlavourData self = ooc_get_interface_must_have( obj, Flavour )->_get_data_( obj );

	self->flavour = flavour;
}

static
const char *
flavour_taste( Object obj )
{
	FlavourData self = ooc_get_interface_must_have( obj, Flavour )->_get_data_( obj );

	return self->flavour ? self->flavour : "flavourless";
}


/* Populating the interface's virtual table
 */

void
Flavour_populate( Flavour flavour )
{
	flavour->set	=	flavour_set;
	flavour->taste	=	flavour_taste;
}
