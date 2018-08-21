#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "str2int.h"


static int compare_by_str( const void *ap, const void *bp )
{
	str2int_pair *a = (str2int_pair *)ap;
	str2int_pair *b = (str2int_pair *)bp;
	return strcmp( a->name, b->name );
}


void str2int_makemap( char **names, int nentries, str2intmap *map )
{
	map->nentries = nentries;
	map->names    = names;
	map->pairs    = malloc( nentries * sizeof(str2int_pair) );
	assert( map->pairs != NULL );

	for( int i=0; i<nentries; i++ )
	{
		map->pairs[i].name  = names[i];
		map->pairs[i].value = i;
	}
	qsort( map->pairs, nentries, sizeof(str2int_pair), &compare_by_str );
}


int str2int_lookup( str2intmap *map, char *name )
{
	str2int_pair key;
	key.name = name;
	str2int_pair *p = bsearch( &key, map->pairs, map->nentries,
			  sizeof(str2int_pair), &compare_by_str );
	assert( p != NULL );
	return p->value;
}
