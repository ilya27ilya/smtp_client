//
//  smtp_fsm.h
//  smtp_client
//
//  Created by mam on 12.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef smtp_fsm_h
#define smtp_fsm_h

#define EMPTY_MESSAGE 1
#define FULL_MESSAGE 0


#define START 0
#define CONNECT 1
#define REC_CONNECT_INFO 2
#define EHLO_STATE 3
#define REC_EHLO 4
#define MAIL_FROM_STATE 5
#define REC_MAIL_FROM 6
#define RCPT_TO_STATE 7
#define REC_RCPT_TO 8
#define DATA_STATE 9
#define REC_DATA 10
#define BODY_STATE 11
#define REC_BODY 12
#define DATA_END_STATE 13
#define REC_DATA_END 14
#define QUIT_STATE 15
#define ERROR_STATE 16



#include <stdio.h>
#include "smtp.h"

int smtp_fsm(int state, int sock, struct message* message, int* mess_state);

#endif /* smtp_fsm_h */
