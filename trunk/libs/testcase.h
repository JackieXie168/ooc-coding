/*
 * testcase.h
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */


#ifndef TESTCASE_H
#define TESTCASE_H

#include <stdio.h>

#include "ooc.h"
#include "exception.h"

DeclareClass( TestCase, Base );

typedef void (*test_method_type)(TestCase);

struct TestCaseMethod
{
	ROM char *			name;
	test_method_type	method;
};

#define TEST(name) {#name, (test_method_type)name}

ClassMembers( TestCase, Base )
	ROM struct TestCaseMethod	*	methods;
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
 
void testcase_fail( ROM char * filename, int line, ROM char * message );
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

#ifdef OOC_HAS_UNIX_SIGNALS
DeclareClass( SegmentationFault, Exception );
DeclareClass( ArithmeticFault, Exception );
#endif

#ifdef OOC_NO_DYNAMIC_MEM
#define PRINT_BUFFER_SIZE 64
#endif

#ifndef TESTCASE_MAIN
#define TESTCASE_MAIN 	int main( int argc, char argv[] )
#endif

#endif  /* TESTCASE_H */