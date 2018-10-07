//
//  message.h
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef message_h
#define message_h

#include <stdio.h>
#include "envelope.h"

struct message{
    
    char*   id;
    struct message_envelope* envelope;
    char*   body;
    
};

struct  message* create_message();
struct  message* fill_message(struct  message* message, char* id, struct message_envelope* envelope, char* body);
void free_message(struct  message* message);

#endif /* message_h */
