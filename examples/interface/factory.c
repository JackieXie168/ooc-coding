
#include "factory.h"

#include <stdlib.h>

size_t
rand_index( int upper_limit )
{
	return rand() % upper_limit;
}

const char *
get_an_owner()
{
	static const char * owners[] = {	"Duffy", "Ted", "Suzy", "Paul", "Peter", "Alice", "Jack",
										"Lemon", "Who?", "Mary", "Anett", "Joe", "Ann", "Edward"
									};

	return owners[ rand_index( sizeof(owners)/sizeof(const char *) ) ];
}

const char *
get_a_date()
{
	static const char * dates[] = {	"2012.01.01", "2011.12.13", "2011.10.28", "2011.08.26", "2011.07.23", "2011.08.01.", "2011.12.06",
									"2011.12.31", "2012.01.02", "2011.02.10", "2011.10.17", "2011.06.10", "2011.09.29", "2011.10.04"
									};

	return dates[ rand_index( sizeof(dates)/sizeof(const char *) ) ];
}

const char *
get_a_text()
{
	static const char * texts[] =
	{
		"text1", 
		"text2", 
		"text3", 
		"text4", 
		"text5", 
		"text6", 
		"text7", 
		"text8"
	};

	return texts[ rand_index( sizeof(texts)/sizeof(const char *) ) ];

}

const char *
get_a_color()
{
	static const char * colors[] = { "yellow", "green", "pink", "white"	};

	return colors[ rand_index( sizeof(colors)/sizeof(const char *) ) ];
}

