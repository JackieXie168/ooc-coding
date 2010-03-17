
#ifndef TESTCLASS_H
#define TESTCLASS_H

#include <ooc.h>

/* Base class for tests
 */
 
DeclareClass( TestBase, Base );

Virtuals( TestBase, Base )

EndOfVirtuals;

/* Test class for storing integers
 */
 
DeclareClass( TestInt, TestBase );

TestInt testint_new( int );

void 	testint_set( TestInt self, int );
int 	testint_get( TestInt self );

Virtuals( TestInt, TestBase )

EndOfVirtuals;

/* Test class for storing strings
 */
  
DeclareClass( TestStr, TestBase );

TestStr teststr_new( const char * );

void 			teststr_set( TestStr self, const char * );
const char * 	teststr_get( TestStr self );

Virtuals( TestStr, TestBase )

EndOfVirtuals;

#endif  /* TESTCLASS_H */
