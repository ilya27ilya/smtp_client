//
//  smtp_fsm.h
//  smtp_client
//
//  Created by mam on 12.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef smtp_fsm_h
#define smtp_fsm_h


#include <stdio.h>

# define CFSM_OK            0
# define CFSM_ERR_INVALID_TRANSITION    -3

typedef enum child_state {
    INIT,
    EHLO_STATE,
    EHLO_REC_STATE,
    READ_MES_STATE,
    QUIT_STATE,
    QUIT_REC_STATE,
    MAIL_FROM_STATE,
    MAIL_FROM_STATE_REC,
    RCPT_TO_STATE,
    RCPT_TO_STATE_REC,
    DATA_STATE,
    DATA_REC_STATE,
    BODDY_STATE,
    BODDY_REC_STATE,
    ERROR_STATE,
    RSET_STATE,
    RSET_REC_STATE,
}child_state;

int smtp_myfsm_advance(child_state *state, int ev);
#endif /* smtp_fsm_h */
