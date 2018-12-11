//
//  smtp.c
//  smtp_client
//
//  Created by mam on 11.11.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "smtp.h"

struct SMTP SMTP;

int send_messages_high(struct message* message, int sock){
    
    int res = 0;
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    printf("Sending EHLO\n");
    SMTP.state = EHLO_COMMAND;
    send_command(sock, EHLO_COMMAND, MY_NAME);
    
    
    
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);
    if (result_code != SUCCESS_OPERATION_CODE)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in send EHLO\n");
        return ERROR_COMAND;
    }
    printf("EHLO send\n");
    
    
    res  = send_message(sock, message);
    if (res == 10)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in sending message");
        return ERROR_COMAND;
    }
    
    
    printf("Sending QUIT\n");
    SMTP.state = QUIT_COMMAND;
    send_command(sock, QUIT_COMMAND, NULL);
    
    bzero(buffer, DATA_BUFFER);
    read(sock, buffer, DATA_BUFFER);
    result_code = strtol(buffer, NULL, 10);
    if (result_code != SUCCESS_QUIT_CODE)
    {
        SMTP.state = ERROR_COMAND;
        printf("Error in send QUIT\n");
        return ERROR_COMAND;
    }
    printf("QUIT send\n");
    
    return res;
}

int send_message(int sock, struct message* message){
    int res = 0;
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
//    SMTP.state = DATA_COMMAND;
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
//    SMTP.state = MESSAGE_BODY_COMMAND;
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
//    SMTP.state = DATA_END_COMMAND;
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
    
    return res;
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


int connect_to_host(char* strServerName){
    struct hostent *he;
    struct sockaddr_in serv_addr;
    int sock;
    char buffer[DATA_BUFFER];
    bzero(buffer, DATA_BUFFER);
    
    he = gethostbyname(strServerName);
    
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
    
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    memcpy(&serv_addr.sin_addr, *(he->h_addr_list), sizeof(struct in_addr));
    
    
    printf("Connect socket\n");
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -2;
    }
    
    read(sock, buffer, DATA_BUFFER);
    long result_code = strtol(buffer, NULL, 10);
    if (result_code != SUCCESS_CONNECTION_CODE)
    {
        printf("Error in connecting\n");
        return ERROR_COMAND;
    }
    printf("Connection esteblished\n");
    

    return sock;
}