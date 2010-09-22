
/* This is a TestCase class implementation file
 */

#include "testcase.h"
#include "exception.h"

/** @class TestCase
 *  @brief TestCase class - base class for unit testing.
 * 	@see testcase.h
 */

/** @file testcase.h 
 *  @brief TestCase class - base class for unit testing.
 * TestCase is a base class for unit testing. All of your Unit tests inherits TestCase.
 * The process to create a new unit test in your project:
 * -# Create a new test class with: ooc -new MyTest --from test
 * -# Override the necessary virtual methods
 * -# implement your test methods
 * -# create the test methods table
 * -# compile your test file as executable
 * -# run the test file
 * 
 */ 

/* Allocating the class description table and the vtable
 */

AllocateClass( TestCase, Base );

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
}

/* Class finalizing
 */

static
void
TestCase_finalize( Class this )
{
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

	self->methods = (const struct TestCaseMethod **) params;
}

/* Destructor
 */

static
void
TestCase_destructor( TestCase self )
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


/*	=====================================================
	Class member functions
 */
 
static int current_test_failed;
static int current_method_fail_count;
static const char * current_method_name;
static const char * current_test_name;

/** Marks the current test method as failed.
 */
 
void
testcase_fail( const char * filename, int line, const char * message )
{
	current_test_failed = TRUE;
	
	if( current_method_fail_count++ == 0 )
		printf( "\n\tFailed: " );
		
	fprintf(stderr, "%s [%s : %d : %s.%s()]", message ? message : "", filename, line, current_test_name, current_method_name );
}

static
void
testcase_run_methods(TestCase self)
{
	const struct TestCaseMethod ** methods = self->methods;
	
	while((*methods)->method)
	{
		current_test_failed = FALSE;
		++self->run;
		
		printf( "\r[%d] %s.before()", self->run , ooc_get_type((Object)self)->name);
		TestCaseVirtual(self)->before(self);
		
		try
		{
			current_method_fail_count = 0;
			current_test_name = ooc_get_type((Object)self)->name;
			current_method_name = (*methods)->name;
			
			printf( "\r[%d] %s.%s()", self->run, ooc_get_type((Object)self)->name, (*methods)->name );
			
			((*methods)->method)(self);
		}
		catch_any {
			printf( "\n\tFailed: " );
			fprintf(stderr, "Unexpected exception %s:%s(), code: %d, user code: %d\n",
							ooc_get_type((Object)exception)->name,
							(*methods)->name,
							exception_get_error_code(exception),
							exception_get_user_code(exception));
			current_test_failed = TRUE;
		}
		end_try;
		
		printf( "\r[%d] %s.after()", self->run, ooc_get_type((Object)self)->name );
		TestCaseVirtual(self)->after(self);
		
		if( current_test_failed )
			self->failed++;
	}	
}

/** Run the TestCase.
 * 
 */

int
testcase_run( TestCase self)
{
	try {
		if( ! ooc_isInstanceOf(self, TestCase) )
			ooc_throw( exception_new(err_bad_cast) );
			
		printf( "\r%s.before_class()", ooc_get_type((Object)self)->name );
		TestCaseVirtual(self)->before_class(self);

		testcase_run_methods(self);
				
		printf( "\r%s.after_class()", ooc_get_type((Object)self)->name );
		TestCaseVirtual(self)->after_class(self);
	}
	catch_any {
		printf( "\n\tFailed: " );
		fprintf(stderr, "Unexpected exception %s in %s, code: %d, user code: %d\n",
						ooc_get_type((Object)exception)->name,
						ooc_get_type((Object)self)->name,
						exception_get_error_code(exception),
						exception_get_user_code(exception));
		self->failed++;
	}
	end_try;
	
	if( self->failed != 0 )
		fprintf(stderr, "Test case %s failed: %d/%d (methods run/failed)\n", ooc_get_type((Object)self)->name, self->run, self->failed );
	
	return 	(self->failed == 0 ) ? 0 : 1;
}
