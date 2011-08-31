#ifndef IMPL_SAMPLE_H
#define IMPL_SAMPLE_H

#ifndef SAMPLE_H
#error Do #include "sample.h" right before doing #include "implement/sample.h"
#endif

typedef
struct
_SampleDataFields
{
	/* Put your mixin's data members here */
	int		data;

} SampleDataFields;

extern	void	Sample_populate( Sample );

extern	void	Sample_constructor( SampleData, x ); /* add additional parameters as needed! */
extern	void	Sample_destructor( SampleData );

#endif /* IMPL_SAMPLE_H */
