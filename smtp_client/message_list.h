//
//  message_list.h
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef message_list_h
#define message_list_h

#include <stdio.h>
#include "message.h"

struct  message_list{
    struct message* message;
    struct message_list* next;
};

struct message_list* create_message_el_list(struct message* message);
struct message_list* add_el_mes_list(struct message_list* mesage_list, struct message_list* message);
//получить элемент по id
void free_message_list(struct message_list* message_list);

#endif /* message_list_h */
