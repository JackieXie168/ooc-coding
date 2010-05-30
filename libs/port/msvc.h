#ifndef MSVC_H_
#define MSVC_H_

/* implementing static inline */
#define STIN static _inline

/* implementing Thread Local Storage */
#define TLS static _declspec( thread )


#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL				\
		__asm {										\
			mov		EBX, ptr_ptr					\
			mov		EAX, 0							\
			xchg	[EBX], EAX						\
			}

#endif /* COMPILING_OOC_C */

#endif /*MSVC_H_*/
