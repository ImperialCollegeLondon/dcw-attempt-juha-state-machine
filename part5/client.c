// ok, my redesign of Juha's state machine: an on/off light switch and light:-)
// now using enums that are defined in client_defs.h and included ABOVE
// state_machine.h everywhere (here in client.c AND in state_machine.c)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "client_defs.h"

#include "state_machine.h"


char *statename[] =		// these MUST be in the same order as state_t
{
	/*NOCHANGE*/    "<NOCHANGE>",
	/*ON*/          "on",
	/*OFF*/         "off",
};

char *actionname[] =		// these MUST be in the same order as action_t
{
	/*BUTTON*/      "button",
	/*TICK*/        "tick",
	/*TOCK*/        "tock",
};


// States and state enter/exit callbacks

static void enter_off( state_t state )
{
	printf( "debug: enter_off: entering state '%s'\n", statename[state] );
}

static void exit_off( state_t state )
{
	printf( "debug: exit_off: entering state '%s'\n", statename[state] );
}

static void enter_on( state_t state )
{
	printf( "debug: enter_on: entering state '%s'\n", statename[state] );
}

static void exit_on( state_t state )
{
	printf( "debug: exit_on: entering state '%s'\n", statename[state] );
}

state_info_t states[] =
{
	{ state_on, &enter_on,  &exit_on },
	{ state_off, &enter_off, &exit_off },
};
#define NUM_STATE_INFO_ENTRIES ( sizeof(states) / sizeof(*states) )


// State Transitions and transition callbacks

static void lights_on( uint16_t t, state_t from, state_t to, action_t action )
{
	printf( "Time %d: LIGHTS ON (%s->%s under %s)\n",
		t, statename[from], statename[to], actionname[action] );
}


static void lights_off( uint16_t t, state_t from, state_t to, action_t action )
{
	printf( "Time %d: LIGHTS OFF (%s->%s under %s)\n",
		t, statename[from], statename[to], actionname[action] );
}


static transition_list_el tldata[] = {
	{ state_off, action_button, state_on, &lights_on },
	{ state_on, action_button, state_off, &lights_off },
};

#define NUM_TRANSITIONS ( sizeof(tldata) / sizeof(*tldata) )


// Now the specific event sequence we want to run the state machine with.

static action_t event_sequence[] =
{
    action_tick, action_tock,
    action_tick, action_tock,
    action_button,
    action_tick, action_tock,
    action_tick, action_tock,
    action_tick, action_tock,
    action_button,
    action_tick, action_tock,
    action_button,
};

#define NUM_EVENTS	(sizeof(event_sequence)/sizeof(*event_sequence))


int main( void )
{
	// tell the state machine the state callbacks
	sm_setstates( states, NUM_STATE_INFO_ENTRIES );

	// add our non-empty transition_tbl entries..
	sm_make_transitions( tldata, NUM_TRANSITIONS );

	sm_transition_callbacks( true );
	sm_run( state_off, event_sequence, NUM_EVENTS );
}
