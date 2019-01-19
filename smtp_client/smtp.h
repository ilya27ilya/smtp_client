//
//  smtp.h
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef smtp_h
#define smtp_h

#include "smtp_fsm.h"
#include <arpa/inet.h>
#include <arpa/nameser.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <resolv.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>

#include "message.h"

#define PORT 25
#define DATA_BUFFER 1024*1024
#define MY_NAME "course_work"

#define EHLO "EHLO "
#define MAIL_FROM "MAIL FROM: "
#define RCPT_TO "RCPT TO: "
#define DATA "DATA"
#define QUIT "QUIT"
#define TERN "\r\n"
#define OPEN_BRACKET "<"
#define CLOSE_BRACKET ">"
#define DATA_END "."
#define RSET "RSET"

#define CONNECTED_ERROR "Error in conection response"

#define INIT_STATE_SMTP 7
#define CONNECTED 8
#define EHLO_COMMAND 0
#define MAIL_FROM_COMMAND 1
#define RCPT_TO_COMMAND 2
#define DATA_COMMAND 3
#define MESSAGE_BODY_COMMAND 4
#define RSET_COMMAND 5
#define QUIT_COMMAND 6
#define ERROR_COMAND 10

#define SUCCESS_EHLO_CODE 220
#define SUCCESS_OPERATION_CODE 250
#define SUCCESS_DATA_SEND_CODE 354
#define SUCCESS_QUIT_CODE 221

int send_command(int sock, child_state command_type, char *load, int *bytes);
int create_socket(const char *host, int port, int attempts_number,
                  int attempts_delay, int *sock);
int try_connect_to_socket(struct sockaddr_in server, int attempts_number,
                          int attempts_delay);
long read_response(int sock);
char *get_mx(char *domain);

#endif /* smtp_h */
