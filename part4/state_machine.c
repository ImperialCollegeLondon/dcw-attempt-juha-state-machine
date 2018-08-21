#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "state_machine.h"
#include "str2int.h"


// Debugging

static bool do_entry_cb = false;
static bool do_exit_cb = false;
static bool do_transition_cb = false;
static bool debug = false;

void sm_debug( bool doit )
{
	debug = doit;
}

void sm_entry_exit_callbacks( bool doentry, bool doexit )
{
	do_entry_cb = doentry;
	do_exit_cb = doexit;
}

void sm_transition_callbacks( bool doit )
{
	do_transition_cb = doit;
}




// States:

static uint16_t num_states;
static char **statenames;
static enter_state_function *entrystate;
static exit_state_function  *exitstate;
str2intmap state_map;

typedef int state_id_t;	// internal code for state

#define STATE_NOCHANGE	0

void sm_setstates( state_info_t *states, int nstates )
{
	num_states = nstates+1;
	statenames = malloc( num_states * sizeof(char *) );
	assert( statenames != NULL );
	entrystate = malloc( num_states * sizeof(enter_state_function) );
	assert( entrystate != NULL );
	exitstate = malloc( num_states * sizeof(exit_state_function) );
	assert( exitstate != NULL );
	statenames[0] = "NOCHANGE";
	entrystate[0] = NULL;
	exitstate[0] = NULL;
	for( int i=0; i<nstates; i++ )
	{
		statenames[i+1] = states[i].name;
		entrystate[i+1] = states[i].on_entry;
		exitstate[i+1] = states[i].on_exit;
	}

	str2int_makemap( statenames, num_states, &state_map );

	#ifdef DEBUG
	for( int i=0; i<state_map.nentries; i++ )
	{
		printf( "debug: statemap[%d]: '%s' == %d\n", i,
			state_map.pairs[i].name,
			state_map.pairs[i].value );
	}
	#endif
}

static state_id_t lookup_state( char *name )
{
	int x = str2int_lookup( &state_map, name );
	assert( x >= 0 && x < num_states );
	return (state_id_t)x;
}


// Actions:  things that can happen eg a button being pressed

typedef int action_id_t;	// internal code for action

static uint16_t num_actions;
static char **actionnames;
str2intmap action_map;

void sm_setactions( char **actions, int nactions )
{
	num_actions = nactions;
	actionnames = actions;
	str2int_makemap( actions, num_actions, &action_map );
}

static action_id_t lookup_action( char *name )
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
		if( debug )
		{
			printf( "debug: event %d '%s' == %d\n", i,
				event_sequence[i], event_id_seq[i] );
		}
	}
}


// State transitions:

// internally we represent state transitions as a 2D array, with everything
// integer-encoded, specifically we store them as an array [ from_state ] of
// array [ action ] of to_state, array [ action ] of transitionfunction pair...
typedef struct {
    state_id_t *newstate;            // array [ action ] of state_id
    transition_function *transition; // array [ action ] of transition_function
} state_transition_t;

static state_transition_t *transition_tbl;


// make the transition_tbl, initially empty, then run through the list of
// transition tuples adding them.
void sm_make_transitions( transition_list_el *list, uint16_t ntransitions )
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


// Now run the state machine with an initial state and an event sequence

void sm_run( state_t initial_statename, char **event_sequence, int nevents )
{
    sm_setevents( event_sequence, nevents );
    state_id_t stateno = lookup_state( initial_statename );
    printf( "Time 0: in state %s\n", initial_statename );

    // iterate over the event sequence
    for( uint16_t e=0; e<num_events; e++ )
    {
	assert( stateno > 0 && stateno < num_states );

	action_id_t action = event_id_seq[e];
	assert( action >= 0 && action < num_actions );

	state_id_t newstate = transition_tbl[stateno].newstate[action];
	transition_function transition = transition_tbl[stateno].transition[action];

	if( debug )
	{
		printf( "debug: time %d: in state %s, event %s, newstate %s\n",
			e,
			statenames[stateno],
			actionnames[action],
			statenames[newstate] );
	}

        if( newstate != STATE_NOCHANGE && newstate != stateno )
	{
                /* true state change */
		exit_state_function ext = exitstate[stateno];
                if( do_exit_cb && ext != NULL )
		{
                    ext( statenames[stateno] );
                }
		if( do_transition_cb && transition != NULL )
		{
			transition( e,
				    statenames[stateno],
				    statenames[newstate],
				    actionnames[action] );
		}

		stateno = newstate;

		enter_state_function ent = entrystate[stateno];
                if( do_entry_cb && ent != NULL )
		{
                    ent( statenames[stateno] );
                }
        }
    }
}
