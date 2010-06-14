#ifndef ANYC_H_
#define ANYC_H_

/* Longjump functions */
 
#include <setjmp.h>

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp


/* implementing static inline */
#define STIN static
#define NO_INLINE

#define NO_THREADS

/* Threadless implementation */

#define TLS

#define	ooc_Mutex				void
#define ooc_mutex_init( x )
#define ooc_mutex_release( x )
#define ooc_lock( x )	
#define ooc_unlock( x )
#define ooc_try_lock( x )		TRUE 


#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL						\
	void * tmp = * ptr_ptr;									\
	* ptr_ptr = NULL;										\
	return tmp;

#endif /* COMPILING_OOC_C */

#endif /*ANYC_H_*/
