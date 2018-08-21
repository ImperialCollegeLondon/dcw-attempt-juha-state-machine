typedef char * state_t;		// names
typedef char * action_t;

typedef int action_id_t;	// internal codes
typedef int state_id_t;

typedef void (*transition_function)( state_t from, state_t to, action_t action );


// transition list:
// the user specifies state transitions as a list of tuples:
// list( from_state, to_state, action, transition_function )
// where from_state, to_state and action are all strings.
typedef struct {
	char *from_state;
	char *action;
	char *to_state;
	transition_function tfunc;
} transition_list_el;


// state transitions: internally we represent state transitions as a 2D
// array, with everything integer-encoded, specifically we store them as
// an array [ from_state ] of array [ action ] of (to_state, transitionfunction)
typedef struct {
    state_id_t *newstate;            // array [ action ] of state_id
    transition_function *transition; // array [ action ] of transition_function
} state_transition_t;

extern void sm_setstates( char ** states, int nstates );
extern state_id_t lookup_state( char * name );
extern void sm_setactions( char ** actions, int nactions );
extern action_id_t lookup_action( char * name );
extern void sm_setevents( char ** event_sequence, int nevents );
extern void make_transitions( transition_list_el * list, uint16_t ntransitions );
