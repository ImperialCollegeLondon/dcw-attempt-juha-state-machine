// ok, my redesign of Juha's state machine: an on/off light switch and light:-)

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

#include "state_machine.h"

char *states[] =
{
	"off",
	"on",
};

#define NUM_STATES ( sizeof(states) / sizeof(char *) )


// things that can happen that the state machine must respond to..
char *actions[] =
{
	"tick",
	"tock",
	"button"
};

#define NUM_ACTIONS ( sizeof(actions) / sizeof(char *) )


// Now the event sequence

static char *event_sequence[] =
{
    "tick",
    "tock",
    "tick",
    "tock",
    "button",
    "tick",
    "tock",
    "tick",
    "tock",
    "tick",
    "tock",
    "button",
    "tick",
    "tock",
    "button",
};

#define NUM_EVENTS	(sizeof(event_sequence)/sizeof(char *))


// Now the transitions

static transition_list_el tldata[] = {
	{ "off", "button", "on", NULL },
	{ "on", "button", "off", NULL },
};

#define NUM_TRANSITIONS ( sizeof(tldata) / sizeof(*tldata) )

int main( void )
{
	sm_setstates( states, NUM_STATES );
	sm_setactions( actions, NUM_ACTIONS );
	sm_setevents( event_sequence, NUM_EVENTS );

	// add our non-empty transition_tbl entries..
	make_transitions( tldata, NUM_TRANSITIONS );

	state_machine( "off" );
}
