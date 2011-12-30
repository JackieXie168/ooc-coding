
#include <ooc/exception.h>

#include "note.h"
#include "postit.h"


int
main( int argc, char * argv[] )
{
	Note	volatile	note	= NULL;
	PostIt	volatile	postit	= NULL;

	ooc_init_class( Exception );

	try
	{
		note = note_new();

		note_set_owner( note, "old_fashioned" );
		note_set_date( note, "2011. 12. 31." );
		note_set_text( note, "Lorem ipsum ..." );

		ooc_get_interface_must_have( note, Serializable )->serialize( (Object) note, 0 );

		postit = postit_new();

		postit_set_color( postit, "yellow" );
		note_set_owner( ooc_cast( postit, Note ), "who?" );
		note_set_date( ooc_cast( postit, Note ), "2011. 12. 30." );
		note_set_text( ooc_cast( postit, Note ), "This is a short memo not to forget ... well, what not to forget?" );

		ooc_get_interface_must_have( postit, Serializable )->serialize( (Object) postit, 0 );
	}
	finally
	{
		ooc_delete( (Object) note );
		ooc_delete( (Object) postit );
	}
	end_try;

	ooc_finalize_all();
}