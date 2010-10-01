
/* This is a TestCase class implementation file
 */

#include "testcase.h"
#include "implement/exception.h"

#include <signal.h>

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

	self->methods = (const struct TestCaseMethod *) params;
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

/** Marks the current test method as failed.
 */
 
void
testcase_fail( const char * filename, int line, const char * message )
{
	current_test_failed = TRUE;
	
	if( current_method_fail_count++ == 0 )
		printf( "\n" );
		
	printf("\tFailed: %s [%s : %u]\n", message ? message : "", filename, line );
}

static
void
testcase_run_methods(TestCase self)
{
	const struct TestCaseMethod * method = self->methods;
	
	while(method->method)
	{
		current_test_failed = FALSE;
		++self->run;
		
		printf( "[%d] %s.before()\r", self->run , ooc_get_type((Object)self)->name);
		TestCaseVirtual(self)->before(self);
		
		try
		{
			current_method_fail_count = 0;
			
			printf( "[%d] %s.%s()\r", self->run, ooc_get_type((Object)self)->name, method->name );
			
			method->method(self);
		}
		catch_any {
			if( ! current_test_failed )
				printf("\n");
			printf("\tUnexpected exception: %s, code: %d, user code: %d\n",
							ooc_get_type((Object)exception)->name,
							exception_get_error_code(exception),
							exception_get_user_code(exception));
			current_test_failed = TRUE;
		}
		end_try;
		
		printf( "[%d] %s.after()\r", self->run, ooc_get_type((Object)self)->name );
		TestCaseVirtual(self)->after(self);
		
		if( current_test_failed )
			self->failed++;
		
		method++;
	}	
}

/* Signal handler prototype
 */
 
static void signal_handler( int signum );

/** Run the TestCase.
 * 
 */

int
testcase_run( TestCase self)
{
	ooc_init_class( SegmentationFault );
	ooc_init_class( ArithmeticFault );
	
	signal( SIGSEGV, signal_handler );
	signal( SIGFPE, signal_handler );
	
	try {
		if( ! ooc_isInstanceOf(self, TestCase) )
			ooc_throw( exception_new(err_bad_cast) );
			
		setbuf(stdout, NULL); /* Unbufferered sdout displays better the current operation */
			
		printf( "%s.before_class()\r", ooc_get_type((Object)self)->name );
		TestCaseVirtual(self)->before_class(self);

		testcase_run_methods(self);
				
		printf( "%s.after_class()\r", ooc_get_type((Object)self)->name );
		TestCaseVirtual(self)->after_class(self);
	}
	catch_any {
		printf("\n\tUnexpected exception %s in %s, code: %d, user code: %d\n",
						ooc_get_type((Object)exception)->name,
						ooc_get_type((Object)self)->name,
						exception_get_error_code(exception),
						exception_get_user_code(exception));
		self->failed++;
	}
	end_try;
	
	if( self->failed != 0 )
		printf("Test case %s failed: %d/%d (methods run/failed)\n", ooc_get_type((Object)self)->name, self->run, self->failed );
	
	return 	(self->failed == 0 ) ? 0 : 1;
}

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

static	void	SegmentationFault_destructor( SegmentationFault self ) {}
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

static	void	ArithmeticFault_destructor( ArithmeticFault self ) {}
static	int		ArithmeticFault_copy( ArithmeticFault self, const ArithmeticFault from ) { return OOC_COPY_DEFAULT; }

/* Signal handler
 */
 
static
void
signal_handler( int signum )
{
	switch( signum ) {
		case SIGSEGV :	signal( SIGSEGV, signal_handler );
						ooc_throw( (Exception) ooc_new( SegmentationFault, NULL ) );
		case SIGFPE	 :	signal( SIGFPE, signal_handler );
						ooc_throw( (Exception) ooc_new( ArithmeticFault, NULL ) );
		default 	 :	break;
	}
}
