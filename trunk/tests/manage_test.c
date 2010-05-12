
/* This is a Foo class implementation file
 */

#include <stdio.h>
#include <exception.h>


DeclareClass( Foo, Base );

ClassMembers( Foo, Base )
EndOfClassMembers;

Virtuals( Foo, Base )
EndOfVirtuals;

AllocateClass( Foo, Base );

static	void	Foo_initialize( Class this ) {}
static	void	Foo_finalize( Class this ) {}

static	void	Foo_constructor( Foo self, const void * params ) 
{
	assert( ooc_isInitialized( Foo ) );
	
	chain_constructor( Foo, self, NULL );
	
	printf( "\tFoo object at %p has been constructed.\n", (void*) self );
}

static	void	Foo_destructor( Foo self )
{
	printf( "\tFoo object at %p has been destroyed.\n", (void*) self );
}

static	int	Foo_copy( Foo self, const Foo from ) { return OOC_COPY_DEFAULT; }

Foo
foo_new( void )
{
	return (Foo) ooc_new( Foo, NULL );		
}

/* This is a FooException class implementation file
 */

#include <implement/exception.h>

DeclareClass( FooException, Exception );

ClassMembers( FooException, Exception )
EndOfClassMembers;

Virtuals( FooException, Exception )
EndOfVirtuals;

AllocateClass( FooException, Exception );

static	void	FooException_initialize( Class this ) {}
static	void	FooException_finalize( Class this ) {}

static	void	FooException_constructor( FooException self, const void * params )
{
	assert( ooc_isInitialized( FooException ) );
	
	chain_constructor( FooException, self, NULL );
	
	self->Exception.user_code = * ((int*) params );
}

static	void	FooException_destructor( FooException self ) {}
static	int	FooException_copy( FooException self, const FooException from ) { return OOC_COPY_DEFAULT; }

Exception
foo_exception_new( int code )
{
	return (Exception) ooc_new( FooException, & code );	
}

static
void
manage_test_2( void )
{
	Foo		foo1;
	Foo		foo2;
	
	foo1	= foo_new();
	{
		ooc_manage_object( foo1 );
		
		foo2	= foo_new();
		{
			ooc_manage_object( foo2 );
			
			printf( "Throwing FooException( 1 )\n" );
			ooc_throw( foo_exception_new( 1 ) );
			
			ooc_delete( (Object) ooc_pass( foo2 ) );
		}
		
		ooc_delete( (Object) ooc_pass( foo1 ) );
	}
}

static
void
manage_test_1( void )
{
	Foo		foo1;
	Foo		foo2;
	
	foo1	= foo_new();
	{
		ooc_manage_object( foo1 );
		
		foo2	= foo_new();
		{
			ooc_manage_object( foo2 );
			
			manage_test_2();
			
			ooc_delete( (Object) ooc_pass( foo2 ) );
		}
		ooc_delete( (Object) ooc_pass( foo1 ) );
	}
}

void
manage_test( void )
{
	ooc_init_class( Exception );
	ooc_init_class( Foo );
	ooc_init_class( FooException );
	
	try {
		manage_test_1( );
		}
	catch( FooException ) {
		printf( "FooException caught, code = %d\n", exception_get_user_code( exception ) );
		}
	catch_any {
		printf( "Unexpected exception caught! Code = %d\n", exception_get_error_code( exception ) );
		}
	end_try;
	
	ooc_finalize_all( );
}
