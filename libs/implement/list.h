#ifndef IMPL_LIST_H_
#define IMPL_LIST_H_

#include "../list.h"

/*   Private class members
 */
 
ClassMembers( List, Base )

    ListIterator		first;
    ListIterator		last;
    
    Class				type;
    list_item_destroyer	destroy;

EndOfClassMembers;

#endif /*IMPL_LIST_H_*/
