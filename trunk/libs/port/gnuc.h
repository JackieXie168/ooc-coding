#ifndef GNUC_H_
#define GNUC_H_

/* implementing static inline */
#if !defined( __NO_INLINE__ )
#define STIN static __inline__
#else 
#define STIN static
#define NO_INLINE
#warning "inline functions are unavailabe"
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
/* pthread should be used */
#warning "no threading library available"
#endif

#else /* threadless implementation */

#define TLS static

typedef struct {} ooc_Nothing;

#define	ooc_Mutex				ooc_Nothing /* allocates 0 bytes, but the name is defined */
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
