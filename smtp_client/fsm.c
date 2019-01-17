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
_is_child_state_valid(enum child_state n)
{
	if (!(n >= INIT && n <= RSET_REC_STATE))
		return -1;
	return 0;
}

const char *
smtp_fsm_state_ntop(enum child_state n)
{
	const char *state_names[] = {
		"INIT",
		"EHLO_STATE",
		"EHLO_REC_STATE",
		"READ_MES_STATE",
		"QUIT_STATE",
		"QUIT_REC_STATE",
		"MAIL_FROM_STATE",
		"MAIL_FROM_STATE_REC",
		"RCPT_TO_STATE",
		"RCPT_TO_STATE_REC",
		"DATA_STATE",
		"DATA_REC_STATE",
		"BODDY_STATE",
		"BODDY_REC_STATE",
		"ERROR_STATE",
		"RSET_STATE",
		"RSET_REC_STATE",
	};

	if (_is_child_state_valid(n) != 0)
		return NULL;
	return state_names[n];
}

const char *
smtp_fsm_state_ntop_safe(enum child_state n)
{
	const char *r = smtp_fsm_state_ntop(n);

	return r == NULL ? "[INVALID]" : r;
}

static int
_is_child_event_valid(enum child_event n)
{
	if (!(n >= AAA && n <= BBB))
		return -1;
	return 0;
}

const char *
smtp_fsm_event_ntop(enum child_event n)
{
	const char *event_names[] = {
		"AAA",
		"BBB",
	};

	if (_is_child_event_valid(n) != 0)
		return NULL;
	return event_names[n];
}

const char *
smtp_fsm_event_ntop_safe(enum child_event n)
{
	const char *r = smtp_fsm_event_ntop(n);

	return r == NULL ? "[INVALID]" : r;
}

int
smtp_fsm_init(struct smtp_fsm *fsm, char *errbuf, size_t errlen)
{
	bzero(fsm, sizeof(fsm));
	fsm->current_state = INIT;
	return CFSM_OK;
}

enum child_state
smtp_fsm_current(struct smtp_fsm *fsm)
{
	return fsm->current_state;
}

int myfsm_advance(struct smtp_fsm *fsm, enum child_event ev,
    char *errbuf, size_t errlen)
{
	enum child_state old_state = fsm->current_state;
	enum child_state new_state;

	/* Sanity check states */
	if (_is_child_state_valid(fsm->current_state) != 0) {
		if (errlen > 0 && errbuf != NULL) {
			snprintf(errbuf, errlen, "Invalid current_state (%d)",
			    fsm->current_state);
		}
		return CFSM_ERR_INVALID_STATE;
	}
	if (_is_child_event_valid(ev) != 0) {
		if (errlen > 0 && errbuf != NULL)
			snprintf(errbuf, errlen, "Invalid event (%d)", ev);
		return CFSM_ERR_INVALID_EVENT;
	}

	/* Event validity checks */
	switch(old_state) {
	case INIT:
		switch (ev) {
		case AAA:
			new_state = EHLO_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case EHLO_STATE:
		switch (ev) {
		case AAA:
			new_state = EHLO_REC_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case EHLO_REC_STATE:
		switch (ev) {
		case AAA:
			new_state = READ_MES_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case READ_MES_STATE:
		switch (ev) {
		case AAA:
			new_state = MAIL_FROM_STATE;
			break;
		case BBB:
			new_state = QUIT_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case QUIT_STATE:
		switch (ev) {
		case AAA:
			new_state = QUIT_REC_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case QUIT_REC_STATE:
		switch (ev) {
		case AAA:
			new_state = EHLO_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case MAIL_FROM_STATE:
		switch (ev) {
		case AAA:
			new_state = MAIL_FROM_STATE_REC;
			break;
		default:
			goto bad_event;
		}
		break;
	case MAIL_FROM_STATE_REC:
		switch (ev) {
		case AAA:
			new_state = RCPT_TO_STATE;
			break;
		case BBB:
			new_state = ERROR_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case RCPT_TO_STATE:
		switch (ev) {
		case AAA:
			new_state = RCPT_TO_STATE_REC;
			break;
		default:
			goto bad_event;
		}
		break;
	case RCPT_TO_STATE_REC:
		switch (ev) {
		case AAA:
			new_state = DATA_STATE;
			break;
		case BBB:
			new_state = ERROR_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case DATA_STATE:
		switch (ev) {
		case AAA:
			new_state = DATA_REC_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case DATA_REC_STATE:
		switch (ev) {
		case AAA:
			new_state = BODDY_STATE;
			break;
		case BBB:
			new_state = ERROR_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case BODDY_STATE:
		switch (ev) {
		case AAA:
			new_state = BODDY_REC_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case BODDY_REC_STATE:
		switch (ev) {
		case AAA:
			new_state = READ_MES_STATE;
			break;
		case BBB:
			new_state = ERROR_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case ERROR_STATE:
		switch (ev) {
		case AAA:
			new_state = RSET_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case RSET_STATE:
		switch (ev) {
		case AAA:
			new_state = RSET_REC_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	case RSET_REC_STATE:
		switch (ev) {
		case AAA:
			new_state = MAIL_FROM_STATE;
			break;
		default:
			goto bad_event;
		}
		break;
	}

	/* Switch state now */
	fsm->current_state = new_state;

	return CFSM_OK;

 bad_event:
	return CFSM_ERR_INVALID_TRANSITION;
}
