
#include "tests.h"

#include "exception.h"

void
manage_test( void )
{
	void * ptr1;
	void * ptr2;
	
	ptr1 = ooc_malloc( 1000 );
	ooc_manage( ptr1, (ooc_destroyer) ooc_free );
	
	ptr2 = ooc_malloc( 2000 );
	ooc_manage( ptr2, (ooc_destroyer) ooc_free );
	
	ooc_free( ooc_pass( ptr2 ) );
	ooc_free( ooc_pass( ptr1 ) );
}

int
main( int argc, char * argv[] )
{
//	vector_test();
//	signal_test();
	
	manage_test();
	
	return 0;
}
