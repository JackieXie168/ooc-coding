
#ifndef TESTCASE_H
#define TESTCASE_H

#include <stdio.h>

#include "ooc.h"
#include "exception.h"

DeclareClass( TestCase, Base );

typedef void (*test_method_type)(TestCase);

struct TestCaseMethod
{
	const char *		name;
	test_method_type	method;
};

#define TEST(name) {#name, (test_method_type)name}

ClassMembers( TestCase, Base )
	const struct TestCaseMethod	*	methods;
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
 
#define fail()							testcase_fail(__FILE__, __LINE__, NULL)
#define assertTrue(expr)				if(!(expr)) testcase_fail(__FILE__, __LINE__, NULL)
#define assertFalse(expr)				if(expr) testcase_fail(__FILE__, __LINE__, NULL)
#define assertNull(expr)				if((expr)!=NULL) testcase_fail(__FILE__, __LINE__, NULL)
#define assertNotNull(expr)				if((expr)==NULL) testcase_fail(__FILE__, __LINE__, NULL)
#define assertZero(expr)				if((expr)!=0) testcase_fail(__FILE__, __LINE__, NULL)
#define assertNotZero(expr)				if((expr)==0) testcase_fail(__FILE__, __LINE__, NULL)

#define failMsg(msg)					testcase_fail(__FILE__, __LINE__, msg)
#define assertTrueMsg(expr, msg)		if(!(expr)) testcase_fail(__FILE__, __LINE__, msg)
#define assertFalseMsg(expr, msg)		if(expr) testcase_fail(__FILE__, __LINE__, msg)
#define assertNullMsg(expr, msg)		if((expr)!=NULL) testcase_fail(__FILE__, __LINE__, msg)
#define assertNotNullMsg(expr, msg)		if((expr)==NULL) testcase_fail(__FILE__, __LINE__, msg)
#define assertZeroMsg(expr, msg)		if((expr)!=0) testcase_fail(__FILE__, __LINE__, msg)
#define assertNotZeroMsg(expr, msg)		if((expr)==0) testcase_fail(__FILE__, __LINE__, msg)

/* Exceptions
 */
 
DeclareClass( SegmentationFault, Exception );
DeclareClass( ArithmeticFault, Exception );

#endif  /* TESTCASE_H */
