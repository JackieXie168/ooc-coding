/*
 * implement/swstack.h
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

#ifndef SWSTACK_H_
#define SWSTACK_H_

extern	   TLS void * *		_ooc_swstack_pointer;
extern	   TLS void * *		_ooc_swstack_end;

STIN
void
ooc_swstack_push( void * p )
{
	if( _ooc_swstack_pointer != _ooc_swstack_end )
		*_ooc_swstack_pointer++ = p;
	else
		ooc_throw( exception_new( err_swstack_overflow ) );
}

STIN
void *
ooc_swstack_pop( void )
{
	return * --_ooc_swstack_pointer;
}

#ifndef OOC_SW_STACK_SIZE
#define OOC_SW_STACK_SIZE 16
#endif

#endif /*SWSTACK_H_*/
