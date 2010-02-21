#ifndef VECTOR_H_
#define VECTOR_H_

#include "ooc.h"

DeclareClass( Vector, Base );

typedef size_t VectorIndex;

typedef void_fn_voidp		vector_item_destroyer;
typedef void_fn_voidp_voidp vector_item_executor;
typedef int_fn_voidp_voidp  vector_item_checker;


/* Vector member functions
 */
 
Vector			vector_new( VectorIndex, vector_item_destroyer );
Vector			vector_new_from_table( void *, size_t, VectorIndex );

VectorIndex		vector_push_back( Vector, void * );
VectorIndex		vector_insert( Vector, VectorIndex, void * );

void			vector_delete_item( Vector, VectorIndex );

VectorIndex		vector_items( Vector );

void            vector_set_item( Vector, VectorIndex, void * );
void *          vector_get_item( Vector, VectorIndex );

void			vector_swap( Vector, VectorIndex, VectorIndex );

void            vector_foreach( Vector, vector_item_executor, void * );
VectorIndex		vector_foreach_until_true( Vector, vector_item_checker, void * );

VectorIndex		vector_find_item        ( Vector, VectorIndex,  vector_item_checker, void * );
VectorIndex		vector_find_item_reverse( Vector, VectorIndex,  vector_item_checker, void * );

#endif /*VECTOR_H_*/
