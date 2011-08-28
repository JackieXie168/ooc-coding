#ifndef SAMPLE_H
#define SAMPLE_H

#include <ooc/ooc.h>

typedef SampleDataFields * SampleData;

DeclareInterface( Sample )

	SampleData	_get_data_( Object );		/* Do not delete! A mixin always must have a _get_data_() method! */

	// Put your interface methods here
	void 	(* my_method1)( Object, ... );

EndOfInterface;

#endif  /* SAMPLE_H */
