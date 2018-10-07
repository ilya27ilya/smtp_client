//
//  message.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "message.h"

struct  message* create_message(){
    struct message* message = (struct message*) malloc(sizeof(struct message));
    if(!message){
        return NULL;
    }
    message->body = NULL;
    message->id = NULL;
    message->envelope = NULL;
    return message;
}


struct  message* fill_message(struct  message* message, char* id, struct message_envelope* envelope, char* body){
    
    if(!message){
        return NULL;
    }
    if(!id){
        return NULL;
    }
    if(!envelope){
        return NULL;
    }
    if(!body){
        return NULL;
    }
    
    message->envelope = (struct message_envelope*)malloc(sizeof(envelope));
    if (!message->envelope) {
        return NULL;
    }
    message->envelope = envelope;
    
    unsigned char tmp = strlen(id);
    message->id = (char*) malloc(tmp+1);
    if(!message->id){
        return NULL;
    }
    strcpy(message->id, id);
    message->id[tmp] = '\0';
    
    unsigned int btmp = strlen(body);
    message->body = (char*) malloc(btmp+1);
    if(!message->body){
        return NULL;
    }
    strcpy(message->body, body);
    message->body[btmp] = '\0';
    
    return message;
}

void free_message(struct  message* message){
    if (message) {
        if (message->body) {
            free(message->body);
            message->body = NULL;
        }
        if (message->id) {
            free(message->id);
            message->id = NULL;
        }
        if (message->envelope) {
            free_envelope(message->envelope);
            message->envelope = NULL;
        }
        free(message);
        message = NULL;
    }
    
}

