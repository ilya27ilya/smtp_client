//
//  programm_manager.h
//  smtp_client
//
//  Created by mam on 11.12.2018.
//  Copyright © 2018 mam. All rights reserved.
//

#ifndef programm_manager_h
#define programm_manager_h

#include <stdlib.h>
#include <netinet/in.h>
#include "smtp.h"
#include "read_message.h"
#include <sys/select.h>
#include "smtp_fsm.h"

#define MAX_MES_NUMBER 3


#define INIT_STATE 0

struct Controller {
    int state;
    int current_message_number;


};

void init_manager();
int Run(int attempts_number, int attempts_delay);


#endif /* programm_manager_h */
