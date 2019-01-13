//
//  smtp.c
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "smtp.h"

struct SMTP SMTP;

int begin_send_messages_high(int sock){
    
    //int res = 0;
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    printf("Sending EHLO\n");
    SMTP.state = EHLO_COMMAND;
    send_command(sock, EHLO_COMMAND, MY_NAME);
    
    
    
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);
    printf("%s\n",buffer);
    if (result_code != SUCCESS_OPERATION_CODE)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in send EHLO\n");
        return ERROR_COMAND;
    }
    printf("EHLO send\n");
    
    return SMTP.state;
    
//    res  = send_message(sock, message);
//    if (res == 10)
//    {
//        SMTP.state = ERROR_COMAND;
//        printf("Error in sending message");
//        return ERROR_COMAND;
//    }
}
int end_send_messages_high(int sock){
    
    //int res = 0;
    char buffer[DATA_BUFFER];
    printf("Sending QUIT\n");
    SMTP.state = QUIT_COMMAND;
    send_command(sock, QUIT_COMMAND, NULL);
    
    bzero(buffer, DATA_BUFFER);
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);
    if (result_code != SUCCESS_QUIT_CODE)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in send QUIT\n");
        return ERROR_COMAND;
    }
    printf("QUIT send\n");
    
    return SMTP.state;
}

int send_message(int sock, struct message* message){
    //int res = 0;
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    printf("Sending message\n");
    
    printf("Send MAIL FROM command\n");
    SMTP.state = MAIL_FROM_COMMAND;
    send_command(sock, MAIL_FROM_COMMAND, message->envelope->sender);
    
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);
    if (result_code != SUCCESS_OPERATION_CODE)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in send MAIL FROM command\n");
        return ERROR_COMAND;
    }
    printf("MAIL FROM send\n");
    
    
    printf("Send RCPT TO command\n");
    SMTP.state = RCPT_TO_COMMAND;
    send_command(sock, RCPT_TO_COMMAND, message->envelope->receiver);
    
    bzero(buffer, DATA_BUFFER);
    read(sock, buffer, DATA_BUFFER);
    result_code = strtol(buffer, NULL, 10);
    if (result_code != SUCCESS_OPERATION_CODE)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in send RCPT TO command\n");
        return ERROR_COMAND;
    }
    printf("RCPT TO send\n");
    
    
//    printf("Send DATA command\n");
    SMTP.state = DATA_COMMAND;
//    send_command(sock, DATA_COMMAND, NULL);
//
//    bzero(buffer, DATA_BUFFER);
//    read(sock, buffer, DATA_BUFFER);
//    result_code = strtol(buffer, NULL, 10);
//    if (result_code != START_SEND_MSG_CODE)
//    {
//        SMTP.state = ERROR_COMAND;
//        printf("Error in send DATA command\n");
//        return ERROR_COMAND;
//    }
//    printf("DATA send\n");
//
//
//
//    printf("Send message body\n");
    SMTP.state = MESSAGE_BODY_COMMAND;
//    send_command(sock, MESSAGE_BODY_COMMAND, message->body);
//
//    bzero(buffer, DATA_BUFFER);
//    read(sock, buffer, DATA_BUFFER);
//    result_code = strtol(buffer, NULL, 10);
//    printf("%s\n",buffer);
//    if (result_code != SUCCESS_DATA_END_CODE)
//    {
//        SMTP.state = ERROR_COMAND;
//        printf("Error in sending message body\n");
//        return ERROR_COMAND;
//    }
//
//    printf("Message body send\n");
//
//
//    printf("Send DATA END command\n");
    SMTP.state = DATA_END_COMMAND;
//    send_command(sock, DATA_END_COMMAND, NULL);
//
//    bzero(buffer, DATA_BUFFER);
//    read(sock, buffer, DATA_BUFFER);
//    result_code = strtol(buffer, NULL, 10);
//    if (result_code != SUCCESS_DATA_END_CODE)
//    {
//        SMTP.state = ERROR_COMAND;
//        printf("Error in send DATA END command\n");
//        return ERROR_COMAND;
//    }
//    printf("DATA END send\n");
    
    printf("Success in sending message\n");
    
    return SMTP.state;
}


int send_command(int sock, int command_type, char* load){
    char command[DATA_BUFFER];
    bzero(command, DATA_BUFFER);
    
    if(command_type == EHLO_COMMAND)
    {
        strncpy(command, EHLO, strlen(EHLO));
        strcat(command, load);
        
    }
    else if (command_type == MAIL_FROM_COMMAND){
        strncpy(command, MAIL_FROM, strlen(MAIL_FROM));
        strcat(command, OPEN_BRACKET);
        strcat(command, load);
        strcat(command, CLOSE_BRACKET);
    }
    else if (command_type == RCPT_TO_COMMAND){
        strncpy(command, RCPT_TO, strlen(RCPT_TO));
        strcat(command, OPEN_BRACKET);
        strcat(command, load);
        strcat(command, CLOSE_BRACKET);
    }
    else if(command_type == DATA_COMMAND){
        strncpy(command, DATA, strlen(DATA));
    }
    else if (command_type == MESSAGE_BODY_COMMAND)
    {
        strncpy(command, load, strlen(load));
    }
    else if(command_type == DATA_END_COMMAND){
        strncpy(command, DATA_END, strlen(DATA_END));
    }
    else if(command_type == QUIT_COMMAND){
        strncpy(command, QUIT, strlen(QUIT));
    }
    
    strcat(command, TERN);
    return write(sock, command, strlen(command));
}

int get_response(int sock, int response_code, int state,char* error_message){
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);
    printf("%s\n",buffer);
    if (result_code != response_code)
    {
        SMTP.state = ERROR_COMAND;
        printf("%s", error_message);
        return ERROR_COMAND;
    }
    SMTP.state = state;
    return SMTP.state;
    
}

int create_socket(const char* host, int port, int attempts_number, int attempts_delay, int* sock)
{
    struct hostent *he;
    struct in_addr **addr_list;
    int socket_found = 0;
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    printf("Try to esteblish connection ");
    printf("%s\n",host);
    
    if ((he = gethostbyname(host )) == NULL)
        return -1;
    
    addr_list = (struct in_addr **) he->h_addr_list;
    
    for(int i = 0; addr_list[i] != NULL && !socket_found; i++)
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(struct sockaddr_in));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(inet_ntoa(*addr_list[i]));
        server.sin_port = htons(PORT);
        
        *sock = try_connect_to_socket(server, attempts_number, attempts_delay);
        if (sock > 0)
        {
            socket_found = 1;
        }
    }
    return 0;
}


int try_connect_to_socket(struct sockaddr_in server, int attempts_number, int attempts_delay)
{
    int sock = socket(AF_INET , SOCK_STREAM , 0);
    int i = 0;
    int result = -1;
    int connection_established = 0;
    while (i < attempts_number && !connection_established)
    {
        int connect_result = connect(sock,(struct sockaddr *)&server, sizeof(server));
        if (connect_result == 0)
        {
            connection_established = 1;
            result = sock;
        }
        else
            sleep(attempts_delay);
        i++;
    }
    return result;
}


long read_response(int sock){
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);

    return result_code;
}
