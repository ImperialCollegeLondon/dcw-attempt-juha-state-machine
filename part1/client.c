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


static char *event_sequences[] =
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

#define NUM_EVENTS	(sizeof(event_sequences)/sizeof(char *))

int main( void )
{
	sm_setstates( states, NUM_STATES );
	sm_setactions( actions, NUM_ACTIONS );
	sm_setevents( event_sequences, NUM_EVENTS );
}
