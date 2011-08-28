
/* This is a Sample interface (mixin) implementation file
 */

#include "sample.h"
#include "implement/sample.h"

#include <ooc/exception.h>

/** @class Sample
 *  @brief Sample interface - brief description.
 * 	@see sample.h
 */

/** @file sample.h
 *  @brief Sample interface - brief description.
 * Sample is a .....
 */

AllocateInterface( Sample );

/* Construction, destruction
 */

void
Sample_constructor( SampleData self )
{
	/* Build up your SampleDataFields.
	 * This method is called ones for every Object, and every field is zeroed an entry.
	 */
	self->data = ?
}

void
Sample_destructor( SampleData self )
{

}


/* Method implementations
 */

static
void
sample_my_method1( Object obj, ... )
{
	SampleData self;

	assert( ooc_isInstanceOf( obj, Base ) );
	self = ooc_get_interface_must_have( obj, Sample )->_get_data_( obj );

	self->data = ...
}


/* Initialization
 */

void
Sample_initialize( Sample sample )
{
	// Fill the Sample virtual table with the virtual methods.
	// Do NOT assign any method to (*_get_data_)() !!!

	sample->my_method1	=	sample_my_method1;
	...
}
