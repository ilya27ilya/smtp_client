//
//  smtp_fsm.c
//  smtp_client
//
//  Created by mam on 12.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "smtp_fsm.h"

int smtp_fsm(int state, int sock, struct message* message, int* mess_state){
    
    long result_code;
    
    switch (state) {
        case START:{
            
           // state = connect();
            /*
             
             
             */
            break;
        }
        case CONNECT:{
            result_code = read_response(sock);
            if (result_code == SUCCESS_CONNECTION_CODE) {
                state = REC_CONNECT_INFO;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_CONNECT_INFO:{
            if(mess_state == FULL_MESSAGE){
                send_command(sock, EHLO_COMMAND, MY_NAME);
                state = EHLO_STATE;
                
            }
            /*
             
             
             */
            break;
        }
        case EHLO_STATE:{
            result_code = read_response(sock);
            if (result_code == SUCCESS_OPERATION_CODE) {
                state = REC_EHLO;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_EHLO:{
            send_command(sock, MAIL_FROM_COMMAND, message->envelope->sender);
            state = MAIL_FROM_STATE;
            /*
             
             
             */
            break;
        }
        case MAIL_FROM_STATE:{
            result_code = read_response(sock);
            if (result_code == SUCCESS_OPERATION_CODE) {
                state = REC_MAIL_FROM;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_MAIL_FROM:{
            send_command(sock, RCPT_TO_COMMAND, message->envelope->receiver);
            state = RCPT_TO_STATE;
            /*
             
             
             */
            break;
        }
        case RCPT_TO_STATE:{
            result_code = read_response(sock);
            if (result_code == SUCCESS_OPERATION_CODE) {
                state = REC_RCPT_TO;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_RCPT_TO:{
            send_command(sock, DATA_COMMAND, NULL);
            state = DATA_STATE;
            /*
             
             
             */
            break;
        }
        case DATA_STATE:{
            result_code = read_response(sock);
            if (result_code == START_SEND_MSG_CODE) {
                state = REC_DATA;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_DATA:{
            send_command(sock, MESSAGE_BODY_COMMAND, message->body);
            state = BODY_STATE;
            /*
             
             
             */
            break;
        }
        case BODY_STATE:{
            result_code = read_response(sock);
            if (result_code == START_SEND_MSG_CODE) {
                state = REC_BODY;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_BODY:{
            send_command(sock, DATA_END_COMMAND, NULL);
            state = DATA_END_STATE;
            /*
             
             
             */
            break;
        }
        case DATA_END_STATE:{
            result_code = read_response(sock);
            if (result_code == SUCCESS_DATA_END_CODE) {
                state = REC_DATA_END;
            } else {
                state = ERROR_STATE;
            }
            break;
        }
        case REC_DATA_END:{
            send_command(sock, QUIT_COMMAND, NULL);
            state = QUIT_STATE;
            /*
             
             
             */
            break;
        }
        case QUIT_STATE:{
            result_code = read_response(sock);
            if (result_code == SUCCESS_OPERATION_CODE) {
                state = REC_EHLO;
                *mess_state = REC_CONNECT_INFO;
            } else {
                state = ERROR_STATE;
            }
            break;
        }

        default:
            return -1;
    }
    return state;
}

