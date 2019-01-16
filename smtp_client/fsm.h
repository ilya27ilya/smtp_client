/*
 * Automatically generated using the cfsm FSM compiler:
 * http://www.mindrot.org/projects/cfsm/
 */

#ifndef _FSM_H
#define _FSM_H

#include <sys/types.h>

/*
 * The valid states of the FSM
 */
enum child_state {
	INIT_STATE,
	HELO_STATE,
	MAIL_STATE,
	RCPT_STATE,
	DATA_STATE,
	ACCEPT_STATE,
};

/*
 * Events that may cause state transitions in the FSM
 */
enum child_event {
	HELO_EV,
	EHLO_EV,
	RSET_EV,
	VRFY_EV,
	QUIT_EV,
	MAIL_EV,
	RCPT_EV,
	DATA_EV,
	DATA_END_EV,
};

/*
 * The FSM object itself.
 */
struct smtp_fsm_transtable;
struct smtp_fsm {
	enum child_state current_state;
	const struct smtp_fsm_transtable *transition_table;
};

/*
 * Possible error return values
 */
#ifndef CFSM_OK
# define CFSM_OK			0
# define CFSM_ERR_INVALID_STATE		-1
# define CFSM_ERR_INVALID_EVENT		-2
# define CFSM_ERR_INVALID_TRANSITION	-3
# define CFSM_ERR_PRECONDITION		-4
#endif /* CFSM_OK */

/*
 * Initialise a FSM and set its starting state to INIT_STATE
 * Will return 0 on success or a CFSM_ERR_* code on failure. 
 * If "errbuf" is not NULL, upto "errlen" bytes of error message
 * will be copied into "errbuf" on failure.
 */
int smtp_fsm_init(struct smtp_fsm *fsm, char *errbuf, size_t errlen);


/*
 * Execute a pre-defined event on the FSM that may trigger a transition.

 * Will return CFSM_OK on success or one of the CFSM_ERR_* codes on failure.
 * If "errbuf" is not NULL, upto "errlen" bytes of error message will be 
 * copied into "errbuf" on failure.
 */
int myfsm_advance(struct smtp_fsm *fsm, enum child_event ev,
    char *errbuf, size_t errlen);

/*
 * Convert from the %(event_enum)s enumeration to a string. Will return
 * NULL if the event is not known.
 */
const char *smtp_fsm_event_ntop(enum child_event);

/*
 * "Safe" version of %(event_enum_to_string_func)s. Will return the string
 * "[INVALID]" if the event is not known, so it can be used directly
 * in printf() statements, etc.
 */
const char *smtp_fsm_event_ntop_safe(enum child_event);

/*
 * Convert from the child_state enumeration to a string. Will return
 * NULL if the state is not known.
 */
const char *smtp_fsm_state_ntop(enum child_state n);

/*
 * "Safe" version of smtp_fsm_state_ntop(). Will return the string
 * "[INVALID]" if the state is not known, so it can be used directly
 * in printf() statements or other contexts where a NULL may be harmful.
 */
const char *smtp_fsm_state_ntop_safe(enum child_state n);

/*
 * Returns the current state of the FSM.
 */
enum child_state smtp_fsm_current(struct smtp_fsm *fsm);

#endif /* _FSM_H */
