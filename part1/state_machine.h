typedef char * state_t;		// names
typedef char * action_t;

typedef int action_id_t;	// internal codes
typedef int state_id_t;

extern void sm_setstates( char ** states, int nstates );
extern void sm_setactions( char ** actions, int nactions );
extern action_id_t lookup_action( char * actionname );
extern void sm_setevents( char ** event_sequence, int nevents );
