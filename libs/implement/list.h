#ifndef IMPL_LIST_H_
#define IMPL_LIST_H_

#include "../list.h"

/*   Private class members
 */
 
ClassMembers( List, Base )

    ListIterator		first;
    ListIterator		last;
    
    int					list_of_nodes;
    Class				type;
    list_item_destroyer	destroy;

EndOfClassMembers;

ClassMembers( ListNode, Base )

	ListNode		next;
	ListNode		previous;

EndOfClassMembers;


#endif /*IMPL_LIST_H_*/

