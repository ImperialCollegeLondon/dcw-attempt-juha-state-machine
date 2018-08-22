/*
 *  str2int.h: a simple immutable string -> integer map (aka hash in Perl terms)
 *	       giving you the ability to quickly lookup what value (integer)
 *	       corresponds to a given string in the map.
 */

typedef struct
{
	char *name;
	int  value;
} str2int_pair;


typedef struct
{
	uint16_t     nentries;
	char         **names;
	str2int_pair *pairs;		// sorted by name
} str2intmap;


extern void str2int_makemap( char ** names, int nentries, str2intmap * map );
extern int str2int_lookup( str2intmap * map, char * name );
