
#ifndef IMPL_TESTCLASS_H
#define IMPL_TESTCLASS_H

#include "../testclass.h"

ClassMembers( TestBase, Base )

EndOfClassMembers;


ClassMembers( TestInt, TestBase )

	int		data;

EndOfClassMembers;


ClassMembers( TestStr, TestBase )

	char *	data;

EndOfClassMembers;


#endif /* IMPL_TESTCLASS_H */
