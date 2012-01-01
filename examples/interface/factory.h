#ifndef FACTORY_H
#define FACTORY_H

#include <stdlib.h>

const char *	get_an_owner();
const char *	get_a_date();
const char *	get_a_text();
const char *	get_a_color();

size_t			rand_index( int upper_limit );

#endif FACTORY_H
