// state_t MUST have an initial state of state_nochange for everything to work

typedef enum { state_nochange, state_on, state_off } state_t;

extern char *statename[];	// in the same order as state_t

#define NUM_STATES     3


typedef enum { action_button, action_tick, action_tock } action_t;

#define NUM_ACTIONS     3

extern char *actionname[];	// in the same order as action_t
