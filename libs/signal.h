#ifndef SIGNAL_H_
#define SIGNAL_H_

#include "list.h"

DeclareClass( Signal, List );

Virtuals( Signal, List )

EndOfVirtuals;

typedef void ( * SignalHandler )( void * target, void * source, void * param );

void		signal_connect( void * source, Signal * signal_p, void * target, SignalHandler handler );

void 		signal_disconnect( void * source, Signal * signal_p, void * target, SignalHandler handler );

void		signal_emit( Signal, void * parameter, ooc_destroyer param_destroy_fn );
void		signal_emit_sync( Signal, void * parameter, ooc_destroyer param_destroy_fn );

void		signal_destroy_notify( Object object );

void		signal_process_signals( void );


#endif /*SIGNAL_H_*/
