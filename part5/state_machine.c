#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "client_defs.h"

#include "state_machine.h"


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

static enter_state_function entrystate[NUM_STATES];
static exit_state_function  exitstate[NUM_STATES];

void sm_setstates( state_info_t *states, int n )
{
	entrystate[0] = NULL;
	exitstate[0] = NULL;
	for( int i=0; i<n; i++ )
	{
		int state = states[i].state;
		entrystate[state] = states[i].on_entry;
		exitstate[state] = states[i].on_exit;
	}
}

// Actions:  things that can happen eg a button being pressed
// [these are already known via client_defs.h]


// Events:  the sequence of actions that DO happen..

static uint16_t num_events;
static action_t *event_seq;

void sm_setevents( action_t *event_sequence, int nevents )
{
	num_events = nevents;
	event_seq = event_sequence;
}


// State transitions:

// internally we represent state transitions as a 2D array, with everything
// integer-encoded, specifically we store them as an array [ from_state ] of
// array [ action ] of to_state, array [ action ] of transitionfunction pair...
typedef struct {
    state_t             newstate[NUM_ACTIONS];   // array [ action ] of state
    transition_function transition[NUM_ACTIONS]; // array [ action ] of transition_function
} state_transition_t;

static state_transition_t transition_tbl[NUM_STATES];


// make the transition_tbl, initially empty, then run through the list of
// transition tuples adding them.
void sm_make_transitions( transition_list_el *list, uint16_t ntransitions )
{
	// initialize everything to no change, no transition function first
	for( int s = 0; s < NUM_STATES; s++ )
	{
		state_transition_t *tp = &transition_tbl[s];
		for( int a = 0; a < NUM_ACTIONS; a++ )
		{
			tp->newstate[a] = state_nochange;
			tp->transition[a] = NULL;
		}
	}
	// now go through the list of non-empty tuples, adding them
	for( int i = 0; i < ntransitions; i++ )
	{
		state_t from = list[i].from_state;
		state_t to = list[i].to_state;
		action_t act = list[i].action;
		transition_function tf = list[i].tfunc;

		transition_tbl[from].newstate[act] = to;
		transition_tbl[from].transition[act] = tf;
	}
}


// Now run the state machine with an initial state and an event sequence

void sm_run( state_t initial_state, action_t *event_sequence, int nevents )
{
    sm_setevents( event_sequence, nevents );
    state_t stateno = initial_state;
    printf( "Time 0: in state %s\n", statename[initial_state] );

    // iterate over the event sequence
    for( uint16_t e=0; e<num_events; e++ )
    {
	assert( stateno > 0 && stateno < NUM_STATES );

	action_t action = event_seq[e];
	assert( action >= 0 && action < NUM_ACTIONS );

	state_t newstate = transition_tbl[stateno].newstate[action];
	transition_function transition = transition_tbl[stateno].transition[action];

	if( debug )
	{
	    printf( "debug: time %d: in state %s, event %s, newstate %s\n",
	    	e,
	    	statename[stateno],
	       	actionname[action],
	    	statename[newstate] );
	}

	if( newstate != state_nochange && newstate != stateno )
	{
	    /* true state change */
	    exit_state_function exitf = exitstate[stateno];
	    if( do_exit_cb && exitf != NULL )
	    {
	    	exitf( stateno );
	    }
	    if( do_transition_cb && transition != NULL )
	    {
	    	transition( e, stateno, newstate, action );
	    }

	    stateno = newstate;
	    //printf( "debug: new state = %d\n", newstate );

	    enter_state_function enterf = entrystate[stateno];
	    if( do_entry_cb && enterf != NULL )
	    {
	    	enterf( stateno );
	    }
	}
    }
}
