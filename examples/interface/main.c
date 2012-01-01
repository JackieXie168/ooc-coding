
#include <ooc/exception.h>

#include "note.h"
#include "postit.h"
#include "booklet.h"


int
main( int argc, char * argv[] )
{
	Note	volatile	note	= NULL;
	PostIt	volatile	postit	= NULL;
	Booklet	volatile	booklet	= NULL;

	ooc_init_class( Exception );

	try
	{
		note = note_new();

		ooc_get_interface_must_have( note, Serializable )->serialize( (Object) note, 0 );

		postit = postit_new();

		ooc_get_interface_must_have( postit, Serializable )->serialize( (Object) postit, 0 );

		booklet = booklet_new();

		ooc_get_interface_must_have( booklet, Serializable )->serialize( (Object) booklet, 0 );
	}
	finally
	{
		ooc_delete( (Object) note );
		ooc_delete( (Object) postit );
		ooc_delete( (Object) booklet );
	}
	end_try;

	ooc_finalize_all();
}