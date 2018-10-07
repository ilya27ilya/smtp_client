//
//  envelope.c
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#include "envelope.h"

struct message_envelope* create_envelope(char* sender, char* receiver, char* subject, char* date)
{
    if (!sender)
        return NULL;
    if (!receiver)
        return NULL;
    if (!subject)
        return NULL;
    if (!date)
        return NULL;
    
    struct message_envelope* envelope = (struct message_envelope*) malloc(sizeof(struct message_envelope));
    
    if (!envelope)
        return NULL;
    
    envelope->date = date;
    envelope->receiver = receiver;
    envelope->sender = sender;
    envelope->subject = subject;
    return envelope;
}

void free_envelope(struct message_envelope* envelope){
    if (envelope) {
        if (envelope->date) {
            //free(envelope->date);
            envelope->date = NULL;
        }
        if (envelope->receiver) {
            //free(envelope->receiver);
            envelope->receiver = NULL;
        }
        if (envelope->subject) {
            //free(envelope->subject);
            envelope->subject = NULL;
        }
        if (envelope->sender) {
            //free(envelope->sender);
            envelope->sender = NULL;
        }
        envelope = NULL;
    }
}
