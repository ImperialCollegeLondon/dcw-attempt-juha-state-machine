// ok, my redesign of Juha's state machine: an on/off light switch and light:-)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "state_machine.h"


// States and state enter/exit callbacks

static void enter_off( state_t state )
{
	printf( "debug: enter_off: entering state '%s'\n", state );
}

static void exit_off( state_t state )
{
	printf( "debug: exit_off: entering state '%s'\n", state );
}

static void enter_on( state_t state )
{
	printf( "debug: enter_on: entering state '%s'\n", state );
}

static void exit_on( state_t state )
{
	printf( "debug: exit_on: entering state '%s'\n", state );
}

state_info_t states[] =
{
	{ "off", &enter_off, &exit_off },
	{ "on",  &enter_on,  &exit_on },
};

#define NUM_STATES ( sizeof(states) / sizeof(*states) )


// Actions: things that can happen..

char *actions[] =
{
	"tick",
	"tock",
	"button"
};

#define NUM_ACTIONS ( sizeof(actions) / sizeof(*actions) )


// State Transitions and transition callbacks

static void lights_on( uint16_t t, state_t from, state_t to, action_t action )
{
	printf( "Time %d: LIGHTS ON (%s->%s under %s)\n", t, from, to, action );
}


static void lights_off( uint16_t t, state_t from, state_t to, action_t action )
{
	printf( "Time %d: LIGHTS OFF (%s->%s under %s)\n", t, from, to, action );
}


static transition_list_el tldata[] = {
	{ "off", "button", "on", &lights_on },
	{ "on", "button", "off", &lights_off },
};

#define NUM_TRANSITIONS ( sizeof(tldata) / sizeof(*tldata) )


/*
 * setup():
 *	Prepare the state machine with our states, actions and
 *	transitions.  Ready to run.
 */
void setup( void )
{
	sm_setstates( states, NUM_STATES );
	sm_setactions( actions, NUM_ACTIONS );

	// add our non-empty transition_tbl entries..
	sm_make_transitions( tldata, NUM_TRANSITIONS );
}


// Now the specific event sequence we want to run the state machine with.

static char *event_sequence[] =
{
    "tick", "tock",
    "tick", "tock",
    "button",
    "tick", "tock",
    "tick", "tock",
    "tick", "tock",
    "button",
    "tick", "tock",
    "button",
};

#define NUM_EVENTS	(sizeof(event_sequence)/sizeof(*event_sequence))


int main( void )
{
	setup();
	sm_transition_callbacks( true );
	sm_run( "off", event_sequence, NUM_EVENTS );
}
