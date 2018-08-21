typedef char * state_t;		// names
typedef char * action_t;

typedef void (*transition_function)( uint16_t time, state_t from, state_t to, action_t action );
typedef void (*exit_state_function)( state_t state );
typedef void (*enter_state_function)( state_t state );

typedef struct {
    char *name;
    enter_state_function on_entry;
    exit_state_function  on_exit;
} state_info_t;


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


extern void sm_debug( bool doit );
extern void sm_entry_exit_callbacks( bool doentry, bool doexit );
extern void sm_transition_callbacks( bool doit );
extern void sm_setstates( state_info_t * states, int nstates );
extern void sm_setactions( char ** actions, int nactions );
extern void sm_setevents( char ** event_sequence, int nevents );
extern void sm_make_transitions( transition_list_el * list, uint16_t ntransitions );
extern void sm_run( state_t initial_statename, char ** event_sequence, int nevents );
