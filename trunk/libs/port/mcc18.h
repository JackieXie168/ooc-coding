/*
 * port/mcc18.h
 * 
 * Copyright (c) 2011, old_fashioned. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#ifndef MCC18_H_
#define MCC18_H_

#ifndef __DEBUG
#define NDEBUG
#endif

/* Inlcude standard headers */

#include <stdlib.h>
#include <stdio.h>

#ifdef NDEBUG
#define assert(e)
#else
#define assert(e) if(!(e)) printf( "*** Assertion failed in %s:%d ***\n", __FILE__,__LINE__ )
#endif

#ifdef COMPILING_OOC_C
#include <stddef.h>
#include <string.h>
#endif /* COMPILING_OOC_C */


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

/* Memory model */

#define OOC_NO_DYNAMIC_MEM 1

#define ROM const rom

#define GEN_PTR	void *


/* What to do in case of an uncaught exception?
 * Let's give abort() a meaning! Hang up ...
 */
#define abort() for(;;)

#ifdef COMPILING_OOC_C

/*	Helper: pointer read-out while nulling
 */

#define OOC_IMPLEMENT_PTR_READ_AND_NULL						\
	void * tmp = * ptr_ptr;									\
	* ptr_ptr = NULL;										\
	return tmp;

#endif /* COMPILING_OOC_C */

#endif /*MCC18_H_*/
