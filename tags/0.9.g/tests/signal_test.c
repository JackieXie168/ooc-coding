
/* This is a SignalTest class implementation file
 */

#include <ooc.h>
#include <exception.h>
#include <signal.h>

#include <stdio.h>

#include "tests.h"

DeclareClass( SignalTest, Base );

Virtuals( SignalTest, Base )

EndOfVirtuals;

ClassMembers( SignalTest, Base )

	int		counter_max;
	int		counter;

	Signal	counted;
	
EndOfClassMembers;

AllocateClass( SignalTest, Base );

/* Class initializing
 */

static
void
SignalTest_initialize( Class this )
{
	ooc_init_class( Signal );
}

/* Class finalizing
 */

static
void
SignalTest_finalize( Class this )
{
}


/* Constructor
 */

static
void
SignalTest_constructor( SignalTest self, const void * params )
{
	assert( ooc_isInitialized( SignalTest ) );
	
	chain_constructor( SignalTest, self, NULL );
	
	self->counter_max = * ( (int*) params );
}

/* Destructor
 */

static
void
SignalTest_destructor( SignalTest self )
{
	signal_destroy_notify( (Object) self );
}

/* Copy constuctor
 */

static
int
SignalTest_copy( SignalTest self, const SignalTest from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_NO_COPY;
}

/*	=====================================================
	Class member functions
 */


SignalTest
signaltest_new( int period )
{
	ooc_init_class( SignalTest );
	
	return (SignalTest) ooc_new( SignalTest, & period );
}


void
signaltest_count( SignalTest self )
{
	assert( ooc_isInstanceOf( self, SignalTest ) );
	
	if( (++ self->counter) >= self->counter_max ) {
		self->counter = 0;
		signal_emit( self->counted, &self->counter_max, NULL );
		}
}

/* This is a Listener class implementation file
 */

DeclareClass( Listener, Base );

Virtuals( Listener, Base )

EndOfVirtuals;

ClassMembers( Listener, Base )

EndOfClassMembers;

AllocateClass( Listener, Base );

/* Class initializing
 */

static
void
Listener_initialize( Class this )
{
	ooc_init_class( Signal );
}

/* Class finalizing
 */

static
void
Listener_finalize( Class this )
{
}


/* Constructor
 */

static
void
Listener_constructor( Listener self, const void * params )
{
	assert( ooc_isInitialized( Listener ) );
	
	chain_constructor( Listener, self, NULL );
}

/* Destructor
 */

static
void
Listener_destructor( Listener self )
{
	signal_destroy_notify( (Object) self );	
}

/* Copy constuctor
 */

static
int
Listener_copy( Listener self, const Listener from )
{
	/* makes the default object copying (bit-by-bit) */
	return OOC_COPY_DEFAULT;
}

/*	=====================================================
	Class member functions
 */


Listener
listener_new( void )
{
	ooc_init_class( Listener );
			
	return (Listener) ooc_new( Listener, NULL );
}


void
listener_print( Listener self, SignalTest source, int * param )
{
#ifndef _OPENMP
	printf( "Signal from source %p reported count %d\n", (void*) source, *param );
#else
	printf( "Signal from source %p reported count %d in thread %d\n", (void*) source, *param, omp_get_thread_num() );
#endif
}

void
signal_test( void )
{
	SignalTest	volatile	test1	= NULL;
	SignalTest	volatile	test2	= NULL;
	Listener	volatile	listener = NULL;
	
	ooc_init_class( Exception );
	ooc_init_class( Signal );
	
	try {
		int i;
		
		test1	= signaltest_new( 17 );
		test2	= signaltest_new( 27 );
		listener = listener_new();
		
		signal_connect ( test1, & test1->counted, listener, (SignalHandler) listener_print );
		signal_connect ( test2, & test2->counted, listener, (SignalHandler) listener_print );
		
		#pragma omp parallel for private(i) schedule( static, 10 )
		for( i=0; i<1000; i++ ) {
			signaltest_count( test1 );
			signaltest_count( test2 );
			if( i % 7  == 0 )
				signal_process_signals();
			}
		signal_process_signals();
		}
	catch_any {
		printf( "Unexpected exception caught! Code = %d\n", exception_get_error_code( exception ) );
		}
	finally {
		ooc_delete( (Object) listener );
		ooc_delete( (Object) test1 );
		ooc_delete( (Object) test2 );
		}
	end_try;
	
	ooc_finalize_all( );
}
