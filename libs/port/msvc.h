#ifndef MSVC_H_
#define MSVC_H_

/* Longjump functions */
 
#include <setjmp.h>

#define JMP_BUF		jmp_buf
#define SETJMP		setjmp
#define LONGJMP		longjmp


/* implementing static inline */
#define STIN static _inline

#ifndef NO_THREADS

/* implementing Thread Local Storage */
#define TLS static _declspec( thread )

#ifdef _OPENMP

#include <omp.h>

#define	ooc_Mutex				omp_lock_t
#define ooc_mutex_init( x )		omp_init_lock( & x )
#define ooc_mutex_release( x )	omp_destroy_lock( & x )
#define ooc_lock( x ) 			omp_set_lock( & x )	
#define ooc_unlock( x ) 		omp_unset_lock( & x )
#define ooc_try_lock( x )		omp_test_lock( & x ) 

#else

#include <Windows.h>

#define	ooc_Mutex				CRITICAL_SECTION
#define ooc_mutex_init( x )		InitializeCriticalSection ( & x )
#define ooc_mutex_release( x )	DeleteCriticalSection( & x )
#define ooc_lock( x ) 			EnterCriticalSection( & x )	
#define ooc_unlock( x ) 		LeaveCriticalSection( & x )
#define ooc_try_lock( x )		TryEnterCriticalSection( & x ) 

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

#define OOC_IMPLEMENT_PTR_READ_AND_NULL				\
			__asm	mov		EBX, ptr_ptr			\
			__asm	mov		EAX, 0					\
			__asm	xchg	[EBX], EAX

#endif /* COMPILING_OOC_C */

#endif /*MSVC_H_*/
