//
//  message_list.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "message_list.h"

struct message_list* create_message_list(struct message* message){
    if(!message){
        return NULL;
    }
    struct message_list* message_list = (struct message_list*)malloc(sizeof(struct message_list));
    
    if (!message_list) {
        return NULL;
    }
    message_list->message = message;
    message_list->next  = NULL;
    return message_list;
}

struct message_list* add_message_element(struct message_list* message_list, struct message* message){
    if(!message_list){
        return NULL;
    }
    if (!message) {
        return message_list;
    }
    struct message_list* item = create_message_list(message);
    message_list->next = item;
    return message_list;
}

