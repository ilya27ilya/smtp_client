//
//  envelope.h
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef envelope_h
#define envelope_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct message_envelope {
    char*   sender;
    char*   receiver;
    char*   subject;
    char*   date;
};

struct message_envelope* create_envelope(char* sender, char* receiver, char* subject, char* date);
void free_envelope(struct message_envelope* envelope);

#endif /* envelope_h */
