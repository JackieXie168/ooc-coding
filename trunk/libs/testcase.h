
#ifndef TESTCASE_H
#define TESTCASE_H

#include <stdio.h>

#include "ooc.h"

DeclareClass( TestCase, Base );

typedef void (*test_method_type)(TestCase);

struct TestCaseMethod
{
	const char *		name;
	test_method_type	method;
};

#define TEST(name) {#name, (test_method_type)name}

ClassMembers( TestCase, Base )
	const struct TestCaseMethod **	methods;
	int								failed;
	int								run;
EndOfClassMembers;


/* Virtual function definitions
 */

Virtuals( TestCase, Base )

	void	(* before_class)  	( TestCase );
	void	(* before)  		( TestCase );
	void	(* after)  			( TestCase );
	void	(* after_class)  	( TestCase );

EndOfVirtuals;

/* Methods
 */
 
void testcase_fail( const char * filename, int line, const char * message );
int testcase_run( TestCase );

/* Assertions
 */
 
#define fail(msg)					testcase_fail(__FILE__, __LINE__, msg)
#define assertTrue(expr, msg)		if(!expr) testcase_fail(__FILE__, __LINE__, msg)
#define assertFalse(expr, msg)		if(expr) testcase_fail(__FILE__, __LINE__, msg)

#endif  /* TESTCASE_H */
