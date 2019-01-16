//
//  read_message.c
//  smtp_client
//
//  Created by mam on 08.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "read_message.h"

struct message* read_message(char* filename){
    FILE* file = fopen(filename,"r");
    if (!file) {
        return NULL;
    }
    struct message* message  = create_message();
    char tmp[BUFFER_SIZE];
    char body[BUFFER_SIZE];
    //char body_p[BODY_SIZE];
    char* id = NULL;
    
    char*   sender = NULL;
    char*   receiver = NULL;
    char*   subject = NULL;
    char*   date = NULL;
    
    unsigned char fl = 0;
    
    while (fgets(tmp, BUFFER_SIZE, file)!=NULL ){
        if(strlen(tmp)==1){
            fl = 1;
            fgets(tmp, BUFFER_SIZE, file);
        }
        if(fl){
            strcat(body, tmp);
        }
        
        
        else{
            if (strncmp(tmp, "Message-ID: ",12) == 0) {
                id = slice_reg(tmp);
            }
            if (strncmp(tmp, "From: ",6) == 0) {
                sender = slice(tmp);
                strcpy(body, "From: ");
                strcat(body, slice_reg(sender));
                
            }
            if (strncmp(tmp, "To: ",4) == 0) {
                receiver = slice(tmp);
                strcat(body,"\nTo: ");
                strcat(body, slice_reg(receiver));
            }
            if (strncmp(tmp, "Subject: ",9) == 0) {
                subject = slice(tmp);
                strcat(body,"\nSubject: ");
                strcat(body, subject);
                strcat(body,"\n\n");
            }
            if (strncmp(tmp, "Date: ",6) == 0) {
                date = slice(tmp);
            
            }
        }
    }
    strcat(body,"\n");
    
    if (!id) {
        return NULL;
    }
    if (!sender) {
        return NULL;
    }
    if (!receiver) {
        return NULL;
    }
    if (!date) {
        return NULL;
    }
    if (!subject) {
        return NULL;
    }
 
//    strcpy(body_p, "From: ");
//    strcat(body_p, sender);
//    strcat(body_p,"\nTo: ");
//    strcat(body_p,receiver);
//    strcat(body_p,"\nSubject: ");
//    strcat(body_p,subject);
//    strcat(body_p,"\n\n");
//    strcat(body_p,body);
//    strcat(body_p,"\n");

    
    
    struct message_envelope* envelope = create_envelope(slice_reg(sender), slice_reg(receiver), subject, date);
    message = fill_body(message, body);
    message = fill_envelope(message, envelope);
    message = fill_id(message, id);
    
    free(id);
    free(receiver);
    free(sender);
    free(date);
    free(subject);
    fclose(file);
    return message;
}
char* slice_reg(char* str){
    char* res=(char*) malloc(BUFFER_SIZE);
    
    char* st = strstr(str, "<");
    long int a = st-str+1;
    
    char* st2 = strstr(str, ">");
    long int b = st2-str+1;
    
    for(long int i = a;i<b-1;i++){
        res[i-a] = str[i];
    }
    return res;
}

char* slice(char* str){
    char* res=(char*) malloc(BUFFER_SIZE);
    
    char* st = strstr(str, ": ");
    long int a = st-str+2;
    
    for(long int i = a;i<strlen(str)-1;i++){
        res[i-a] = str[i];
    }
    return res;
}



