//
//  smtp_fsm.c
//  smtp_client
//
//  Created by mam on 12.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "smtp_fsm.h"

int smtp_myfsm_advance(child_state *state, int ev)
{
    
    /* Event validity checks */
    switch(*state) {
        case INIT:
            switch (ev) {
                case 1:
                    *state = EHLO_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case EHLO_STATE:
            switch (ev) {
                case 1:
                    *state = EHLO_REC_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case EHLO_REC_STATE:
            switch (ev) {
                case 1:
                    *state = READ_MES_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case READ_MES_STATE:
            switch (ev) {
                case 1:
                    *state = MAIL_FROM_STATE;
                    break;
                case 0:
                    *state = QUIT_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case QUIT_STATE:
            switch (ev) {
                case 1:
                    *state = QUIT_REC_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case QUIT_REC_STATE:
            switch (ev) {
                case 1:
                    *state = EHLO_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case MAIL_FROM_STATE:
            switch (ev) {
                case 1:
                    *state = MAIL_FROM_STATE_REC;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case MAIL_FROM_STATE_REC:
            switch (ev) {
                case 1:
                    *state = RCPT_TO_STATE;
                    break;
                case 0:
                    *state = ERROR_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case RCPT_TO_STATE:
            switch (ev) {
                case 1:
                    *state = RCPT_TO_STATE_REC;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case RCPT_TO_STATE_REC:
            switch (ev) {
                case 1:
                    *state = DATA_STATE;
                    break;
                case 0:
                    *state = ERROR_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case DATA_STATE:
            switch (ev) {
                case 1:
                    *state = DATA_REC_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case DATA_REC_STATE:
            switch (ev) {
                case 1:
                    *state = BODDY_STATE;
                    break;
                case 0:
                    *state = ERROR_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case BODDY_STATE:
            switch (ev) {
                case 1:
                    *state = BODDY_REC_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case BODDY_REC_STATE:
            switch (ev) {
                case 1:
                    *state = READ_MES_STATE;
                    break;
                case 0:
                    *state = ERROR_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case ERROR_STATE:
            switch (ev) {
                case 1:
                    *state = RSET_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case RSET_STATE:
            switch (ev) {
                case 1:
                    *state = RSET_REC_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        case RSET_REC_STATE:
            switch (ev) {
                case 1:
                    *state = MAIL_FROM_STATE;
                    break;
                default:
                    goto bad_event;
            }
            break;
        default:
            goto bad_event;
    }
    
    
    /* Switch state now */
    
    return CFSM_OK;
    
bad_event:
    return CFSM_ERR_INVALID_TRANSITION;
}

