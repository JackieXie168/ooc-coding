
/* This is a IceCream class implementation file
 */

#include "icecream.h"
#include "implement/icecream.h"

#include <ooc/exception.h>

/* Allocating the class description table and the vtable
 */

InterfaceRegister( IceCream )
{
	AddInterface( IceCream, Flavour )
};

AllocateClassWithInterface( IceCream, Tonic );

/* Class virtual function prototypes
 */

static
FlavourData
icecream_flavourdata( Object self )
{
	return & ( (IceCream) self )->Flavour;
}


/* Class initializing
 */

static
void
IceCream_initialize( Class this )
{
	IceCreamVtable vtab = (IceCreamVtable) this->vtable;
	
	vtab->Flavour._get_data_ = icecream_flavourdata;
	Flavour_populate( & vtab->Flavour );
}

/* Class finalizing
 */

#ifndef OOC_NO_FINALIZE
static
void
IceCream_finalize( Class this )
{
}
#endif


/* Constructor
 */

static
void
IceCream_constructor( IceCream self, const void * flavour )
{
	assert( ooc_isInitialized( IceCream ) );
	
	chain_constructor( IceCream, self, NULL );

	Flavour_constructor( & self->Flavour, (const char *) flavour );
}

/* Destructor
 */

static
void
IceCream_destructor( IceCream self, IceCreamVtable vtab )
{
	Flavour_destructor( & self->Flavour );
}

/* Copy constuctor
 */

static
int
IceCream_copy( IceCream self, const IceCream from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


IceCream
icecream_new( const char * flavour )
{
	ooc_init_class( IceCream );
		
	return ooc_new( IceCream, flavour );
}
