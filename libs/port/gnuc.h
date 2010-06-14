#ifndef GNUC_H_
#define GNUC_H_

/* Longjump functions */
 
#include <setjmp.h>

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp


/* implementing static inline */
#if !defined( __NO_INLINE__ )
#define STIN static __inline__
#else 
#define STIN static
#define NO_INLINE
#endif

#ifndef NO_THREADS

/* implementing Thread Local Storage */
#define TLS static __thread

#ifdef _OPENMP

#include <omp.h>

#define	ooc_Mutex				omp_lock_t
#define ooc_mutex_init( x )		omp_init_lock( & x )
#define ooc_mutex_release( x )	omp_destroy_lock( & x )
#define ooc_lock( x ) 			omp_set_lock( & x )	
#define ooc_unlock( x ) 		omp_unset_lock( & x )
#define ooc_try_lock( x )		omp_test_lock( & x ) 

#else

#include <pthread.h>

#define	ooc_Mutex				pthread_mutex_t
#define ooc_mutex_init( x )		pthread_mutex_init( & x, NULL )
#define ooc_mutex_release( x )	pthread_mutex_destroy( & x )
#define ooc_lock( x ) 			pthread_mutex_lock( & x )	
#define ooc_unlock( x ) 		pthread_mutex_unlock( & x )
#define ooc_try_lock( x )		pthread_mutex_trylock( & x ) 

#endif

#else /* threadless implementation */

#define TLS static

#define	ooc_Mutex				void
#define ooc_mutex_init( x )
#define ooc_mutex_release( x )
#define ooc_lock( x )	
#define ooc_unlock( x )
#define ooc_try_lock( x )		TRUE 

#endif


#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL			\
	void * ret_val;								\
	__asm__ (									\
		  "movl		%1, %%ebx;"					\
		  "movl		$0, %0;"					\
		  "xchgl	%0, (%%ebx)"				\
				: "=a" (ret_val)				\
				: "m" (ptr_ptr)					\
				: "ebx" );						\
	return ret_val;

#endif /* COMPILING_OOC_C */

#endif /*GNUC_H_*/
