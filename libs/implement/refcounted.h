#ifndef IMPL_REFCOUNTED_H_
#define IMPL_REFCOUNTED_H_

#include "../refcounted.h"

ClassMembers( RefCounted, Base )

	int			counter;

#ifndef NO_THREADS
	ooc_Mutex	access;
#endif

EndOfClassMembers;

#endif /* IMPL_REFCOUNTED_H_ */
