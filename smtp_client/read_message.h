//
//  read_message.h
//  smtp_client
//
//  Created by mam on 08.10.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef read_message_h
#define read_message_h

#include "message.h"
#include <stdio.h>

#define BUFFER_SIZE 1024*1024
#define BODY_SIZE 1024*1024
#define ENVELOPE_SIZE 500

struct message *read_message(char *filename);
char *slice(char *str);
char *slice_reg(char *str);
#endif /* read_message_h */
