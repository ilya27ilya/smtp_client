/*
 * Automatically generated using the cfsm FSM compiler:
 * http://www.mindrot.org/projects/cfsm/
 */

#include <sys/types.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "fsm.h"

static int
_is_mystate_valid(enum mystate n)
{
	if (!(n >= INIT_STATE && n <= ACCEPT_STATE))
		return -1;
	return 0;
}

const char *
myfsm_state_ntop(enum mystate n)
{
	const char *state_names[] = {
		"INIT_STATE",
		"HELO_STATE",
		"MAIL_STATE",
		"RCPT_STATE",
		"DATA_STATE",
		"ACCEPT_STATE",
	};

	if (_is_mystate_valid(n) != 0)
		return NULL;
	return state_names[n];
}

const char *
myfsm_state_ntop_safe(enum mystate n)
{
	const char *r = myfsm_state_ntop(n);

	return r == NULL ? "[INVALID]" : r;
}

static int
_is_myevent_valid(enum myevent n)
{
	if (!(n >= HELO_EV && n <= DATA_END_EV))
		return -1;
	return 0;
}

const char *
myfsm_event_ntop(enum myevent n)
{
	const char *event_names[] = {
		"HELO_EV",
		"EHLO_EV",
		"RSET_EV",
		"VRFY_EV",
		"QUIT_EV",
		"MAIL_EV",
		"RCPT_EV",
		"DATA_EV",
		"DATA_END_EV",
	};

	if (_is_myevent_valid(n) != 0)
		return NULL;
	return event_names[n];
}

const char *
myfsm_event_ntop_safe(enum myevent n)
{
	const char *r = myfsm_event_ntop(n);

	return r == NULL ? "[INVALID]" : r;
}

int
myfsm_init(struct myfsm *fsm, char *errbuf, size_t errlen)
{
	bzero(fsm, sizeof(fsm));
	fsm->current_state = INIT_STATE;
	return CFSM_OK;
}

enum mystate
myfsm_current(struct myfsm *fsm)
{
	return fsm->current_state;
}

int myfsm_advance(struct myfsm *fsm, enum myevent ev,
    char *errbuf, size_t errlen)
{
	enum mystate old_state = fsm->current_state;
	enum mystate new_state;

	/* Sanity check states */
	if (_is_mystate_valid(fsm->current_state) != 0) {
		if (errlen > 0 && errbuf != NULL) {
			snprintf(errbuf, errlen, "Invalid current_state (%d)",
			    fsm->current_state);
		}
		return CFSM_ERR_INVALID_STATE;
	}
	if (_is_myevent_valid(ev) != 0) {
		if (errlen > 0 && errbuf != NULL)
			snprintf(errbuf, errlen, "Invalid event (%d)", ev);
		return CFSM_ERR_INVALID_EVENT;
	}

	/* Event validity checks */
	switch(old_state) {
	case INIT_STATE:
		switch (ev) {
		case HELO_EV:
			new_state = HELO_STATE;
			break;
		case EHLO_EV:
			new_state = HELO_STATE;
			break;
		case RSET_EV:
			new_state = INIT_STATE;
			break;
		case VRFY_EV:
			new_state = INIT_STATE;
			break;
		case QUIT_EV:
			new_state = ACCEPT_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case HELO_STATE:
		switch (ev) {
		case MAIL_EV:
			new_state = MAIL_STATE;
			break;
		case RSET_EV:
			new_state = HELO_STATE;
			break;
		case VRFY_EV:
			new_state = HELO_STATE;
			break;
		case QUIT_EV:
			new_state = ACCEPT_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case MAIL_STATE:
		switch (ev) {
		case RCPT_EV:
			new_state = RCPT_STATE;
			break;
		case RSET_EV:
			new_state = HELO_STATE;
			break;
		case VRFY_EV:
			new_state = MAIL_STATE;
			break;
		case QUIT_EV:
			new_state = ACCEPT_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case RCPT_STATE:
		switch (ev) {
		case DATA_EV:
			new_state = DATA_STATE;
			break;
		case RCPT_EV:
			new_state = RCPT_STATE;
			break;
		case RSET_EV:
			new_state = HELO_STATE;
			break;
		case VRFY_EV:
			new_state = RCPT_STATE;
			break;
		case QUIT_EV:
			new_state = ACCEPT_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case DATA_STATE:
		switch (ev) {
		case DATA_END_EV:
			new_state = HELO_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case ACCEPT_STATE:
		goto bad_event;
	}

	/* Switch state now */
	fsm->current_state = new_state;

	return CFSM_OK;

 bad_event:
	if (errlen > 0 && errbuf != NULL) {
		snprintf(errbuf, errlen,
		    "Invalid event %s in state %s",
		    myfsm_event_ntop_safe(ev),
		    myfsm_state_ntop_safe(fsm->current_state));
	}
	return CFSM_ERR_INVALID_TRANSITION;
}