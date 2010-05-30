#ifndef IMPL_REFCOUNTED_H_
#define IMPL_REFCOUNTED_H_

#include "../refcounted.h"

ClassMembers( RefCounted, Base )

	int			counter;
	ooc_Mutex	access;

EndOfClassMembers;

#endif /* IMPL_REFCOUNTED_H_ */
