
#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "ooc.h"

/** Exception class declaration
 */
 
DeclareClass( Exception, Base );

/** Error codes.
 * These error codes are included in the Exceptions thorwn by the ooc core.
 */
 
enum
error_codes
{
	err_no_error		= 0,			/**< no error */
	err_out_of_memory,					/**< no memory for allocating an object */
	err_bad_throw,						/**< bad use of throw (rethow whithout try, wrong code, etc.) */
	err_bad_cast,						/**< bad cast of an object */
	err_undefined_virtual,				/**< calling of an undefined virtual function */
	err_can_not_be_duplicated,			/**< duplication of an object is invalid */
	err_wrong_position,					/**< insertion at wrong position into a container */

	err_user_code						/**< Non ooc core exception == user defined exception */
};

/* Class methods */

Exception	exception_new( enum error_codes );
int			exception_get_error_code( const Exception );
int			exception_get_user_code( const Exception );

/*	Exception handling
 */

void		ooc_throw( Exception );
void		ooc_rethrow( void ); 

struct ooc_try_block {
	Exception				exc_obj;
	int						status;
	struct ooc_try_block *	previous;
	jmp_buf					buffer;
	};

#define		try			{ struct ooc_try_block TryContext;		\
						Exception exception;					\
						ooc_link_try_block( &TryContext );		\
						if( ! setjmp(TryContext.buffer) )
#define		catch(ec)	else if( (exception = ooc_exception_caught( & ec ## Class ))) 
#define		catch_any	else if( (exception = ooc_exception_caught( NULL )))
#define		finalize	exception = NULL;	/* Only for supressing "unused variable warning" with a pure try .. finalize ..end_try block */
#define		end_try		ooc_end_try(); }

void		ooc_link_try_block( struct ooc_try_block * );
Exception	ooc_exception_caught( const Class );
void		ooc_end_try( void );


/* Virtual function definitions
 */

Virtuals( Exception, Base )

	/* No virtuals, yet */

EndOfVirtuals;

#endif  /* EXCEPTION_H */

