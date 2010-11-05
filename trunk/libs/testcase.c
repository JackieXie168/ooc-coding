
/* This is a TestCase class implementation file
 */

#include "testcase.h"
#include "implement/exception.h"

#include <signal.h>
#include <string.h>

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

static ooc_Mutex	printing;

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

	self->methods = (const struct TestCaseMethod *) params;
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
	
	ooc_lock( printing );

	if( current_method_fail_count++ == 0 )
		printf( "\n" );
		
	printf("\tFailed: %s [%s : %u]\n", message ? message : "", filename, line );

	ooc_unlock( printing );
}

static const char * before_class = "before_class";
static const char * after_class = "after_class";

static
void
print_func_name( TestCase self, const char * func, const char * suffix )
{
	int			buffer_length;
	int			display_length;
	static int	previous_display_length = 0;
	char *	volatile display_text = NULL;
	
	try {
		ooc_lock( printing );
		
		buffer_length = 32 + strlen( func ) + 1 + strlen( suffix ) + 1 + strlen( ooc_get_type((Object)self)->name ) + 3;
		
		if( buffer_length < previous_display_length + 1 )
			buffer_length = previous_display_length + 1;
		
		display_text = ooc_malloc( buffer_length );
		
		if( func == before_class || func == after_class )
			sprintf( display_text,  "%s.%s()", ooc_get_type((Object)self)->name, func );
		else if( strlen( suffix ) == 0 )
			sprintf( display_text,  "[%d] %s.%s()", self->run , ooc_get_type((Object)self)->name, func );
		else
			sprintf( display_text,  "[%d] %s.%s.%s()", self->run , ooc_get_type((Object)self)->name, func, suffix );
		
		display_length = strlen( display_text );
		if( display_length < previous_display_length ) {
			memset( &display_text[ display_length ], ' ', previous_display_length - display_length );
			display_text[ previous_display_length ] = '\0';
			}
	
		previous_display_length = strlen( display_text );
			
		printf( "%s\r", display_text );
	}
	finally {
		ooc_free( display_text );
		ooc_unlock( printing );
	}
	end_try;
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
		
		print_func_name( self, method->name, "before" );
		TestCaseVirtual(self)->before(self);
		
		try
		{
			current_method_fail_count = 0;
			
			print_func_name( self, method->name, "" );
			
			method->method(self);
		}
		catch_any {
			ooc_lock( printing );
			
			if( ! current_test_failed )
				printf("\n");
			printf("\tUnexpected exception: %s, code: %d, user code: %d\n",
							ooc_get_type((Object)exception)->name,
							exception_get_error_code(exception),
							exception_get_user_code(exception));
			current_test_failed = TRUE;
			ooc_unlock( printing );
		}
		end_try;
		
		print_func_name( self, method->name, "after" );
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
			
		print_func_name( self, before_class, "" );
		TestCaseVirtual(self)->before_class(self);

		testcase_run_methods(self);
				
		print_func_name( self, after_class, "" );
		TestCaseVirtual(self)->after_class(self);
	}
	catch_any {
		ooc_lock( printing );
		printf("\n\tUnexpected exception %s in %s, code: %d, user code: %d\n",
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
		printf("Test case %s failed: %d/%d (methods run/failed)\n", ooc_get_type((Object)self)->name, self->run, self->failed );
		ooc_unlock( printing );
	}
	
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
