//
//  smtp.h
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef smtp_h
#define smtp_h

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <fcntl.h>


#include "message.h"

#define PORT 25
#define DATA_BUFFER 2048
#define MY_NAME "smtp_i_client"

#define EHLO "EHLO "
#define MAIL_FROM "MAIL FROM: "
#define RCPT_TO "RCPT TO: "
#define DATA "DATA"
#define QUIT "QUIT"
#define TERN "\r\n"
#define OPEN_BRACKET "<"
#define CLOSE_BRACKET ">"
#define DATA_END "."


#define EHLO_COMMAND 0
#define MAIL_FROM_COMMAND 1
#define RCPT_TO_COMMAND 2
#define DATA_COMMAND 3
#define MESSAGE_BODY_COMMAND 4
#define DATA_END_COMMAND 5
#define QUIT_COMMAND 6
#define ERROR_COMAND 10


#define START_SEND_MSG_CODE 354
#define SUCCESS_QUIT_CODE 221
#define SUCCESS_OPERATION_CODE 250

#define SUCCESS_DATA_END_CODE 451
#define SUCCESS_CONNECTION_CODE 220

//структура
struct SMTP {
    int state;
};

//отправить письмо
//создание команды

int send_messages_high(struct message* message, int sock);
int send_command(int sock, int command_type, char* load);
int send_message(int sock, struct message* message);
int create_socket(const char* host, int port, int attempts_number, int attempts_delay);
int try_connect_to_socket(struct sockaddr_in server, int attempts_number, int attempts_delay);
#endif /* smtp_h */
