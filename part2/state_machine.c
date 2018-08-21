#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "state_machine.h"
#include "str2int.h"


// States:

static uint16_t num_states;
static char **statenames;
str2intmap state_map;

void sm_setstates( char **states, int nstates )
{
	num_states = nstates+1;
	statenames = malloc( num_states * sizeof(char *) );
	assert( statenames != NULL );
	statenames[0] = "NOCHANGE";
	for( int i=0; i<nstates; i++ )
	{
		statenames[i+1] = states[i];
	}

	str2int_makemap( statenames, num_states, &state_map );

	for( int i=0; i<state_map.nentries; i++ )
	{
		printf( "debug: statemap[%d]: '%s' == %d\n", i,
			state_map.pairs[i].name,
			state_map.pairs[i].value );
	}
}

state_id_t lookup_state( char *name )
{
	int x = str2int_lookup( &state_map, name );
	assert( x >= 0 && x < num_states );
	return (state_id_t)x;
}


// Actions:  things that can happen eg a button being pressed

static uint16_t num_actions;
static char **actionnames;
str2intmap action_map;

void sm_setactions( char **actions, int nactions )
{
	num_actions = nactions;
	actionnames = actions;
	str2int_makemap( actions, num_actions, &action_map );
}

action_id_t lookup_action( char *name )
{
	int x = str2int_lookup( &action_map, name );
	assert( x >= 0 && x < num_actions );
	return (action_id_t)x;
}


// Events:  the sequence of actions that DO happen..

static uint16_t num_events;
static action_id_t *event_id_seq;

void sm_setevents( char **event_sequence, int nevents )
{
	num_events = nevents;
	event_id_seq = malloc( nevents * sizeof(action_id_t) );
	assert( event_id_seq != NULL );
	for( int i=0; i<nevents; i++ )
	{
		event_id_seq[i] = lookup_action( event_sequence[i] );
		printf( "debug: event %d '%s' == %d\n", i,
			event_sequence[i], event_id_seq[i] );
	}
}


// State transitions:

static state_transition_t *transition_tbl;


// make the transition_tbl, initially empty, then run through the list of
// transition tuples adding them.
void make_transitions( transition_list_el *list, uint16_t ntransitions )
{
	transition_tbl = (state_transition_t *)
		malloc( num_states*sizeof(state_transition_t) );
	assert( transition_tbl != NULL );

	// initialize everything to no change, no transition function first
	for( int s = 0; s < num_states; s++ )
	{
		state_id_t *sp =
			(state_id_t *) malloc( num_actions*sizeof(state_id_t) );
		assert( sp != NULL );
		transition_tbl[s].newstate = sp;

		transition_function *tp =
			(transition_function *)
			malloc( num_actions*sizeof(transition_function) );
		assert( tp != NULL );
		transition_tbl[s].transition = tp;
		for( int a = 0; a < num_actions; a++ )
		{
			sp[a] = lookup_state( "NOCHANGE" );
			tp[a] = NULL;
		}
	}
	// now go through the list of non-empty tuples, adding them
	for( int i = 0; i < ntransitions; i++ )
	{
		state_id_t from = lookup_state( list[i].from_state );
		state_id_t to = lookup_state( list[i].to_state );
		action_id_t act = lookup_action( list[i].action );
		transition_function tf = list[i].tfunc;

		transition_tbl[from].newstate[act] = to;
		transition_tbl[from].transition[act] = tf;
	}
}
