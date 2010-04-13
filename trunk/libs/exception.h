
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
	err_bad_connect,					/**< signal connection failure */

	err_user_code		= 0x100			/**< Non ooc core exception == user defined exception */
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
	struct ooc_Manageable * managed;
	};

#define		try			{ struct ooc_try_block TryContext;		\
						Exception exception;					\
						ooc_link_try_block( &TryContext );		\
						if( ! setjmp(TryContext.buffer) )
#define		catch(ec)	else if( (exception = ooc_exception_caught( & ec ## Class ))) 
#define		catch_any	else if( (exception = ooc_exception_caught( NULL )))
#define		finally		exception = NULL;	/* Only for supressing "unused variable warning" with a pure try .. finallly ..end_try block */
#define		end_try		ooc_end_try(); }

void		ooc_link_try_block( struct ooc_try_block * );
Exception	ooc_exception_caught( const Class );
void		ooc_end_try( void );

struct ooc_Manageable
{
	void *		target;
	void		(* destroyer)( void * );
	struct ooc_Manageable * previous;
};

void		ooc_chain_manageable( struct ooc_Manageable * );

#define ooc_manage( target, destroyer ) \
	{ struct ooc_Manageable manageable = { target, destroyer }; ooc_chain_manageable( & manageable ); }
	
#define ooc_manage_object( target ) \
	{ struct ooc_Manageable manageable = { target, (ooc_destroyer) ooc_delete }; ooc_chain_manageable( & manageable ); }
	
#ifdef NDEBUG
void		ooc_unchain_last_manageable( void );
#define		ooc_pass( x )	( ooc_unchain_last_manageable(), x )
#else
void		ooc_unchain_manageable( void * target );
#define		ooc_pass( x )	( ooc_unchain_manageable( x ), x )
#endif

/* Virtual function definitions
 */

Virtuals( Exception, Base )

	/* No virtuals, yet */

EndOfVirtuals;

#endif  /* EXCEPTION_H */

