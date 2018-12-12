//
//  programm_manager.c
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "programm_manager.h"

struct Controller Manager;

void init_manager(){
    Manager.state = INIT_STATE;
    
    Manager.ya_message = EMPTY_MESSAGE;
    Manager.ma_message = EMPTY_MESSAGE;
    Manager.go_message = EMPTY_MESSAGE;
    Manager.current_message_number = 0;
    Manager.ya_state = INIT_STATE_SMTP;
    Manager.ma_state = INIT_STATE_SMTP;
    Manager.go_state = INIT_STATE_SMTP;
    
}



int Run(int attempts_number, int attempts_delay){

    int sock_ya;
    create_socket("mx.yandex.ru", PORT, attempts_number, attempts_delay,&sock_ya);
    if(sock_ya < 0){
        return -1;
    }
    int sock_go;
    create_socket("gmail-smtp-in.l.google.com", PORT, attempts_number, attempts_delay,&sock_go);
    if(sock_go < 0){
        return -1;
    }
    int sock_ma;
    create_socket("mxs.mail.ru", PORT, attempts_number, attempts_delay, &sock_ma);
    if(sock_ma < 0){
        return -1;
    }

    Node *head = NULL;
    push(&head, "/Users/mam/xcode/smtp_client/maildir/new/1.txt");
    push(&head, "/Users/mam/xcode/smtp_client/maildir/new/2.txt");
    push(&head, "/Users/mam/xcode/smtp_client/maildir/new/3.txt");
    push(&head, "/Users/mam/xcode/smtp_client/maildir/new/4.txt");
    push(&head, "/Users/mam/xcode/smtp_client/maildir/new/5.txt");

    struct message* ya_message = create_message();
    struct message* ma_message = create_message();
    struct message* go_message = create_message();
    char* file_name;

    while(1){

        struct message* new_message = create_message();
        if(head != NULL){
            file_name = pop(&head);
        }
        else
        {
            file_name = NULL;
        }

        if(Manager.current_message_number <= MAX_MES_NUMBER && file_name != NULL){
            new_message = read_message(file_name);

            if(strstr(new_message->envelope->receiver,"@mail") != NULL && Manager.ma_message){
                ma_message = new_message;
                Manager.ya_message = FULL_MESSAGE;
            }
            else if(strstr(new_message->envelope->receiver,"@ya") != NULL && Manager.ma_message){
                ya_message = new_message;
                Manager.ya_message = FULL_MESSAGE;
            }
            else if(strstr(new_message->envelope->receiver,"@gm") != NULL && Manager.ma_message){
                go_message = new_message;
                Manager.ya_message = FULL_MESSAGE;
            }
            else{
                push(&head, file_name);
            }
        }

        fd_set socket_set;
        FD_ZERO(&socket_set);
        FD_SET(sock_ma, &socket_set);
        FD_SET(sock_go, &socket_set);
        FD_SET(sock_ya, &socket_set);
        struct timeval timeout;
        timeout.tv_sec  = 10;
        timeout.tv_usec = 0;

        //printf("+");
        if (select(sock_go+1, &socket_set, NULL, NULL, &timeout) > 0){

            if (FD_ISSET(sock_ma, &socket_set) && Manager.ma_state == INIT_STATE_SMTP){
                Manager.ma_state = get_response(sock_ma, SUCCESS_CONNECTION_CODE, CONNECTED,CONNECTED_ERROR);
                printf("1");
                send_command(sock_ma, EHLO_COMMAND, MY_NAME);
                Manager.ma_state = EHLO_COMMAND;
            }

            if (FD_ISSET(sock_ya, &socket_set) && Manager.ya_state == INIT_STATE_SMTP){
                Manager.ya_state = get_response(sock_ya, SUCCESS_CONNECTION_CODE, CONNECTED,CONNECTED_ERROR);
                printf("2");
                send_command(sock_ya, EHLO_COMMAND, MY_NAME);
                Manager.ya_state = EHLO_COMMAND;
            }

            if (FD_ISSET(sock_go, &socket_set) && Manager.go_state == INIT_STATE_SMTP){
                Manager.go_state = get_response(sock_go, SUCCESS_CONNECTION_CODE, CONNECTED,CONNECTED_ERROR);
                printf("3");
                send_command(sock_go, EHLO_COMMAND, MY_NAME);
                Manager.go_state = EHLO_COMMAND;
            }

        }



    }

    close(sock_ya);
    close(sock_go);
    close(sock_ma);

    return 0;
}



void push(Node **head, char* data) {
    Node *tmp = (Node*) malloc(sizeof(Node));
    tmp->value = data;
    tmp->next = (*head);
    (*head) = tmp;
}


char* pop(Node **head) {
    Node* prev = NULL;
    char* val;
    if (head == NULL) {
        return NULL;
    }
    prev = (*head);
    val = prev->value;
    (*head) = (*head)->next;
    free(prev);
    return val;
}
