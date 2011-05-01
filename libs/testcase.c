/*
 * testcase.c
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


/* This is a TestCase class implementation file
 */

#include "testcase.h"
#include "exception.h"
#include "implement/exception.h"

#ifdef OOC_HAS_UNIX_SIGNALS
#include <signal.h>
#endif

#include <string.h>

/** @class TestCase
 *  @brief TestCase class - base class for unit testing.
 * 	@see testcase.h
 */

/** @file testcase.h 
 *  @brief TestCase class - base class for unit testing.
 * TestCase is a base class for unit testing. All of your Unit tests inherits TestCase.
 * The process to create a new unit test in your project:
 * -# Create a new test class with: ooc --new MyTest --source test
 * -# Override the necessary virtual methods
 * -# implement your test methods
 * -# create the test methods table
 * -# compile your test file as executable
 * (you can use the makefile template from ooc/template/unit folder)
 * -# run the test file
 * 
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( TestCase, Base );

#ifndef NO_THREADS
static ooc_Mutex	printing;
#endif

/* Class virtual function prototypes
 */

static
void
_testcase_default_virtual( TestCase self )
{
	return;
}


/* Class initializing
 */

static
void
TestCase_initialize( Class this )
{
	TestCaseVtable vtab = (TestCaseVtable) this->vtable;
	
	vtab->before_class	= 	_testcase_default_virtual;
	vtab->before		= 	_testcase_default_virtual;
	vtab->after			= 	_testcase_default_virtual;
	vtab->after_class	= 	_testcase_default_virtual;
	
	ooc_mutex_init( printing );
}

/* Class finalizing
 */

static
void
TestCase_finalize( Class this )
{
	ooc_mutex_release( printing );
}


/* Constructor
 */

static
void
TestCase_constructor( TestCase self, const void * params )
{
	if(!ooc_isInitialized( TestCase )) {
		ooc_throw(exception_new(err_uninitialized));
	}
	
	chain_constructor( TestCase, self, NULL );

	self->methods = (ROM struct TestCaseMethod *) params;
}

/* Destructor
 */

static
void
TestCase_destructor( TestCase self, TestCaseVtable vtab )
{
}

/* Copy constuctor
 */

static
int
TestCase_copy( TestCase self, const TestCase from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/* Signal handler prototype
 */
 
#ifdef OOC_HAS_UNIX_SIGNALS
static void signal_handler( int signum );
#endif


/*	=====================================================
	Class member functions
 */
 
static int current_test_failed;
static int current_method_fail_count;

/** Marks the current test method as failed.
 */
 
void
testcase_fail( ROM char * filename, int line, ROM char * message )
{
	current_test_failed = TRUE;
	
	ooc_lock( printing );

	if( current_method_fail_count++ == 0 )
		printf( "\n" );
		
	printf("\tFailed: %HS [%HS : %u]\n", message ? message : "", filename, line );

	ooc_unlock( printing );
}

static ROM char before_class[] = "before_class";
static ROM char after_class[] = "after_class";

#ifdef OOC_NO_DYNAMIC_MEM
char print_buffer[PRINT_BUFFER_SIZE];
#endif

static
void
print_func_name( TestCase self, ROM char * func, ROM char * suffix )
{
#ifndef OOC_NO_DYNAMIC_MEM
	int			buffer_length;
#endif
	int			display_length;
	static int	previous_display_length = 0;
	char *	volatile display_text = NULL;
	
	try {
		ooc_lock( printing );
		
#ifndef OOC_NO_DYNAMIC_MEM
		if( func != NULL )
			buffer_length = 32 + strlen( func ) + 1 + strlen( suffix ) + 1 + strlen( ooc_get_type((Object)self)->name ) + 3;
		else
			buffer_length = 1;
			
		if( buffer_length < previous_display_length + 1 )
			buffer_length = previous_display_length + 1;
		
		display_text = ooc_malloc( buffer_length );
#else
		display_text = print_buffer;
#endif
		
		if( func == NULL )
			display_text[0] = '\0';
		else if( func == before_class || func == after_class )
			sprintf( display_text,  "%HS.%HS()", ooc_get_type((Object)self)->name, func );
		else if( suffix == NULL )
			sprintf( display_text,  "[%d] %HS.%HS()", self->run , ooc_get_type((Object)self)->name, func );
		else 
			sprintf( display_text,  "[%d] %HS.%HS.%HS()", self->run , ooc_get_type((Object)self)->name, func, suffix );
		
		display_length = strlen( display_text );
		if( display_length < previous_display_length ) {
			memset( &display_text[ display_length ], ' ', previous_display_length - display_length );
			display_text[ previous_display_length ] = '\0';
			}
	
		previous_display_length = strlen( display_text );
			
		printf( "%s\r", display_text );
	}
	finally {
#ifndef OOC_NO_DYNAMIC_MEM
		ooc_free( display_text );
#endif
		ooc_unlock( printing );
	}
	end_try;
}

static
void
testcase_run_before_class_recursive(TestCase self, Class type)
{
	TestCaseVtable vtab = (TestCaseVtable) type->vtable;
	
	if( ooc_class_has_parent( type ) )
		testcase_run_before_class_recursive( self, type->parent );
	
	vtab->before_class(self);
}

static
void
testcase_run_after_class_recursive(TestCase self, Class type)
{
	TestCaseVtable vtab = (TestCaseVtable) type->vtable;
	
	vtab->after_class( self );
	
	if( ooc_class_has_parent( type ) )
		testcase_run_after_class_recursive( self, type->parent );
}

static
void
testcase_run_before_recursive(TestCase self, Class type)
{
	TestCaseVtable vtab = (TestCaseVtable) type->vtable;
	
	if( ooc_class_has_parent( type ) )
		testcase_run_before_recursive( self, type->parent );
	
	vtab->before(self);
}

static
void
testcase_run_after_recursive(TestCase self, Class type)
{
	TestCaseVtable vtab = (TestCaseVtable) type->vtable;
	
	vtab->after( self );
	
	if( ooc_class_has_parent( type ) )
		testcase_run_after_recursive( self, type->parent );
}

static
void
testcase_run_methods(TestCase self)
{
	ROM struct TestCaseMethod * method = self->methods;
	
	while(method->method)
	{
		current_test_failed = FALSE;
		++self->run;
		current_method_fail_count = 0;

		try
		{
			print_func_name( self, method->name, "before" );
			testcase_run_before_recursive( self, ooc_get_type( (Object) self ) );
		
			print_func_name( self, method->name, NULL );
			method->method(self);
			
			print_func_name( self, method->name, "after" );
			testcase_run_after_recursive( self, ooc_get_type( (Object) self ) );
		}
		catch_any {
			ooc_lock( printing );
			
			if( ! current_test_failed )
				printf("\n");
			printf("\tUnexpected exception: %HS, code: %d, user code: %d\n",
							ooc_get_type((Object)exception)->name,
							exception_get_error_code(exception),
							exception_get_user_code(exception));
			current_test_failed = TRUE;
			ooc_unlock( printing );
		}
		end_try;
		
		if( current_test_failed )
			self->failed++;
		
		method++;
	}	
}

/** Run the TestCase.
 * 
 */

int
testcase_run( TestCase self)
{
#ifdef OOC_HAS_UNIX_SIGNALS
	ooc_init_class( SegmentationFault );
	ooc_init_class( ArithmeticFault );
	
	signal( SIGSEGV, signal_handler );
	signal( SIGFPE, signal_handler );
#endif

	try {
		if( ! ooc_isInstanceOf(self, TestCase) )
			ooc_throw( exception_new(err_bad_cast) );
			
		setbuf(stdout, NULL); /* Unbufferered stdout displays better the current operation */
			
		print_func_name( self, before_class, "" );
		testcase_run_before_class_recursive( self, ooc_get_type( (Object) self ) );

		testcase_run_methods(self);
				
		print_func_name( self, after_class, "" );
		testcase_run_after_class_recursive( self, ooc_get_type( (Object) self ) );
		
		print_func_name( self, NULL, NULL ); /* following a succesfull run cleans the last function name */
	}
	catch_any {
		ooc_lock( printing );
		printf("\n\tUnexpected exception %HS in %HS, code: %d, user code: %d\n",
						ooc_get_type((Object)exception)->name,
						ooc_get_type((Object)self)->name,
						exception_get_error_code(exception),
						exception_get_user_code(exception));
		self->failed++;
		ooc_unlock( printing );
	}
	end_try;
	
	if( self->failed != 0 ) {
		ooc_lock( printing );
		printf("Test case %HS failed: %d/%d (methods run/failed)\n", ooc_get_type((Object)self)->name, self->run, self->failed );
		ooc_unlock( printing );
	}
	
	return 	(self->failed == 0 ) ? 0 : 1;
}

#ifdef OOC_HAS_UNIX_SIGNALS

/** Segmentation fault exception.
*/

ClassMembers( SegmentationFault, Exception )
EndOfClassMembers;

Virtuals( SegmentationFault, Exception )
EndOfVirtuals;

AllocateClass( SegmentationFault, Exception );

static	void	SegmentationFault_initialize( Class this ) {}
static	void	SegmentationFault_finalize( Class this ) {}

static	void	SegmentationFault_constructor( SegmentationFault self, const void * params )
{
	assert( ooc_isInitialized( SegmentationFault ) );
	
	chain_constructor( SegmentationFault, self, NULL );
}

static	void	SegmentationFault_destructor( SegmentationFault self, SegmentationFaultVtable vtab ) {}
static	int		SegmentationFault_copy( SegmentationFault self, const SegmentationFault from ) { return OOC_COPY_DEFAULT; }

/** Arithmetic fault exception.
*/

ClassMembers( ArithmeticFault, Exception )
EndOfClassMembers;

Virtuals( ArithmeticFault, Exception )
EndOfVirtuals;

AllocateClass( ArithmeticFault, Exception );

static	void	ArithmeticFault_initialize( Class this ) {}
static	void	ArithmeticFault_finalize( Class this ) {}

static	void	ArithmeticFault_constructor( ArithmeticFault self, const void * params )
{
	assert( ooc_isInitialized( ArithmeticFault ) );
	
	chain_constructor( ArithmeticFault, self, NULL );
}

static	void	ArithmeticFault_destructor( ArithmeticFault self, ArithmeticFaultVtable vtab ) {}
static	int		ArithmeticFault_copy( ArithmeticFault self, const ArithmeticFault from ) { return OOC_COPY_DEFAULT; }

/* Unix Signal handler
 */
 
static
void
signal_handler( int signum )
{
	signal( SIGSEGV, signal_handler );
	signal( SIGFPE, signal_handler );
	
	switch( signum ) {
		case SIGSEGV :	ooc_throw( (Exception) ooc_new( SegmentationFault, NULL ) );
		case SIGFPE	 :	ooc_throw( (Exception) ooc_new( ArithmeticFault, NULL ) );
		default 	 :	break;
	}
}

#endif /* OOC_HAS_UNIX_SIGNALS */
