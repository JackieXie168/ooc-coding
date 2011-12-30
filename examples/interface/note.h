
#ifndef NOTE_H
#define NOTE_H

#include <ooc/ooc.h>

#include "serializable.h"

DeclareClass( Note, Base );

Note note_new( void );

void note_set_owner( Note self, const char * owner );
void note_set_date( Note self, const char * date );
void note_set_text( Note self, const char * text );

/* Virtual function definitions
 */

Virtuals( Note, Base )

	Interface( Serializable );

EndOfVirtuals;

#endif  /* NOTE_H */
