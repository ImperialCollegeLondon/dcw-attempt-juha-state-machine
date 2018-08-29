/*
 * state_machine.h:	a generic simple state machine with states, actions
 *			and an event sequence (a list of actions).  all are
 *			represented in client-land as strings, but mapped to
 *			integers inside the state machine for efficiency.
 */

// This code only compiles in the presence of two predefined
// enumerated types: state_t and action_t.

typedef void (*transition_function)( uint16_t time, state_t from, state_t to, action_t action );
typedef void (*exit_state_function)( state_t state );
typedef void (*enter_state_function)( state_t state );


// a state information type:
typedef struct {
	state_t state;
	enter_state_function on_entry;
	exit_state_function on_exit;
} state_info_t;


// transition list:
// the user specifies state transitions as a list of tuples:
// each ( from_state, to_state, action, transition_function )
// where from_state, to_state and action are all strings.
// a transition list is passed into the state machine via
// sm_make_transitions() where it is converted into an efficient
// 2D array of integers.
typedef struct {
	state_t from_state;
	action_t action;
	state_t to_state;
	transition_function tfunc;
} transition_list_el;


extern void sm_debug( bool doit );
extern void sm_entry_exit_callbacks( bool doentry, bool doexit );
extern void sm_transition_callbacks( bool doit );
extern void sm_setstates( state_info_t * states, int n );
extern void sm_setevents( action_t * event_sequence, int nevents );
extern void sm_make_transitions( transition_list_el * list, uint16_t ntransitions );
extern void sm_run( state_t initial_state, action_t * event_sequence, int nevents );
