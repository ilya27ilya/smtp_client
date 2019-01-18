//
//  message.h
//  smtp_client
//
//  Created by mam on 07.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef message_h
#define message_h

#include "envelope.h"
#include <stdio.h>

struct message {

  char *id;
  struct message_envelope *envelope;
  char *body;
};

struct message *create_message();
struct message *fill_message(struct message *message, char *id,
                             struct message_envelope *envelope, char *body);
void free_message(struct message *message);
struct message *fill_envelope(struct message *message,
                              struct message_envelope *envelope);
struct message *fill_id(struct message *message, char *id);
struct message *fill_body(struct message *message, char *body);
#endif /* message_h */
